#include "chatserver.h"

#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <qlowenergydescriptordata.h>

#include <QTimer>
#include <QDebug>
#include <QCoreApplication>
#include <QBluetoothDeviceDiscoveryAgent>

ChatServer::ChatServer(QObject *parent)
    : QObject(parent)
{
}

ChatServer::~ChatServer()
{
    stopServer();
}

void ChatServer::startServer(const QBluetoothUuid &serviceUuid,
                             const QBluetoothUuid &rxCharUuid,
                             const QBluetoothUuid &txCharUuid,
                             const QBluetoothUuid &reverseServiceUuid,
                             const QBluetoothUuid &reverseRxCharUuid,
                             const QBluetoothUuid &reverseTxCharUuid)
{
    if (controller) {
        qWarning() << "BLE ChatServer already running";
        return;
    }

    qDebug() << "Starting BLE ChatServer (peripheral)...";

    this->rxUuid = rxCharUuid;
    this->txUuid = txCharUuid;

    this->reverseServiceUuid = reverseServiceUuid;
    this->reverseRxUuid = reverseRxCharUuid;
    this->reverseTxUuid = reverseTxCharUuid;

    controller = QLowEnergyController::createPeripheral(this);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatServer::onConnectionStateChanged);

    // connect(controller, &QLowEnergyController::connected,
    //        this, &ChatServer::startCentral);

    connect(controller, &QLowEnergyController::errorOccurred,
            this, [this](QLowEnergyController::Error error) {
                emit serverError(QStringLiteral("Controller error: %1").arg(error));
            });

    // RX (client -> server)
    QLowEnergyCharacteristicData rxData;
    rxData.setUuid(rxUuid);
    rxData.setProperties(QLowEnergyCharacteristic::Write | QLowEnergyCharacteristic::WriteNoResponse);
    rxData.setValue(QByteArray());

    // TX (server -> client)
    QLowEnergyCharacteristicData txData;
    txData.setUuid(txUuid);
    txData.setProperties(QLowEnergyCharacteristic::Notify);
    txData.setValue(QByteArray());
    // **ADD CCC descriptor**
    QLowEnergyDescriptorData ccc(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration, QByteArray(2, 0x00));
    txData.addDescriptor(ccc);

    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(serviceUuid);
    serviceData.addCharacteristic(rxData);
    serviceData.addCharacteristic(txData);

    service = controller->addService(serviceData);
    if (!service) {
        emit serverError("Failed to create BLE service");
        stopServer();
        return;
    }

    qDebug() << "[Server] GATT table:";
    for (const auto &c : service->characteristics()) {
        qDebug() << " -" << c.uuid()
        << "props:" << c.properties()
        << "valid:" << c.isValid();
    }

    // CACHE CHARACTERISTICS (CRITICAL)
    for (const auto &c : service->characteristics()) {
        if (c.uuid() == rxUuid)
            rxChar = c;
        else if (c.uuid() == txUuid)
            txChar = c;
    }

    if (!rxChar.isValid() || !txChar.isValid()) {
        emit serverError("RX or TX characteristic missing after service creation");
        stopServer();
        return;
    }

    // IMPORTANT (cross-platform correctness):
    // In the PERIPHERAL role, an incoming write from the connected central
    // (our client's move) is delivered via characteristicChanged() — NOT
    // characteristicWritten(). Per Qt docs, characteristicWritten() "is only
    // emitted for Central Role related use cases", so connecting it here never
    // fires for incoming writes. This was why the single-link design received
    // nothing on macOS. characteristicChanged() fires on both macOS/iOS
    // (CoreBluetooth) and Android when a GATT client writes our RX.
    connect(service, &QLowEnergyService::characteristicChanged,
            this, &ChatServer::onCharacteristicWritten);

    // Advertising
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(
        QLowEnergyAdvertisingData::DiscoverabilityGeneral
        );
    advertisingData.setServices({ serviceUuid });

    // Keep advertising packet small!
    QLowEnergyAdvertisingData scanResponseData;
    scanResponseData.setLocalName("Gobblet S");

    controller->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData, scanResponseData);

    qDebug() << "Advertising BLE service:" << serviceUuid.toString();
}

void ChatServer::stopServer()
{
    if (!controller)
        return;

    qDebug() << "Stopping BLE ChatServer";

    if (service) {
        service->disconnect(this);
        service->deleteLater();
        service = nullptr;
    }

    if (controller) {
        controller->disconnect(this);
        controller->disconnectFromDevice();
        controller->stopAdvertising();
        controller->deleteLater();
        controller = nullptr;
    }

    rxChar = QLowEnergyCharacteristic();
    txChar = QLowEnergyCharacteristic();

    if (centralService) {
        centralService->disconnect(this);
        centralService->deleteLater();
        centralService = nullptr;
    }

    if (centralController) {
        centralController->disconnect(this);
        centralController->disconnectFromDevice();
        centralController->deleteLater();
        centralController = nullptr;
    }

    centralTxChar = QLowEnergyCharacteristic();
}

void ChatServer::onCharacteristicWritten(const QLowEnergyCharacteristic &c,
                                         const QByteArray &value)
{
    // Driven by QLowEnergyService::characteristicChanged() in peripheral role:
    // fired whenever a connected central writes one of our characteristics.
    // We only care about writes to the RX characteristic (client -> server).
    if (c.uuid() != rxUuid) {
        qDebug() << "[Server] Ignoring write to non-RX characteristic"
                 << "uuid:" << c.uuid()
                 << "expected rxUuid:" << rxUuid;
        return;
    }

    qDebug() << "[Server] RX WRITE EVENT"
             << "uuid:" << c.uuid()
             << "valid:" << c.isValid()
             << "props:" << c.properties()
             << "value Hex:" << value.toHex()
             << "value ascii:" << QString::fromUtf8(value);

    qDebug() << "[Server] RX RECEIVED";
    emit messageReceived("Client", QString::fromUtf8(value));
}

void ChatServer::sendMessage(const QString &message)
{
    if (!service || !txChar.isValid()) {
        qWarning() << "Cannot send message: TX characteristic invalid";
        return;
    }

    qDebug() << "Sending BLE notification:" << message;

    // Send a GATT notification on the peripheral's Notify characteristic.
    // Do NOT pass WriteWithoutResponse here: on a local (peripheral) service Qt
    // treats a plain writeCharacteristic() on a Notify characteristic as a
    // notification to subscribed clients. Passing a write mode works on iOS
    // (CoreBluetooth maps it internally) but fails to deliver on Android.
    service->writeCharacteristic(txChar, message.toUtf8());
}

void ChatServer::onConnectionStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
        case QLowEnergyController::ConnectedState:
            qDebug() << "BLE central connected to our peripheral";
            // Store the connected client's address/UUID so we can find it
            // during reverse-channel discovery. On Android/Linux remoteAddress()
            // returns a real MAC; on macOS/iOS it is null but remoteDeviceUuid()
            // provides a stable CoreBluetooth identifier.
            connectedClientAddress = controller->remoteAddress();
            connectedClientUuid   = controller->remoteDeviceUuid();
            qDebug() << "[Server] Connected client address:" << connectedClientAddress.toString()
                     << "uuid:" << connectedClientUuid.toString();
            emit clientConnected("BLE Central");
            // NOTE: The reverse central (scan + connect back to the client
            // peripheral) is no longer needed. The client now sends its moves by
            // writing to our forward RX characteristic over the existing GATT
            // link (see ChatClient::sendMessage). Starting a scan + second GATT
            // connection here while acting as a peripheral was unreliable on
            // Android and is intentionally disabled.
            // startCentral();
            break;

        case QLowEnergyController::UnconnectedState:
            qDebug() << "BLE central disconnected";
            emit clientDisconnected("BLE Central");
            break;

        default:
            break;
    }
}

void ChatServer::startCentral() {
    qDebug() << "Start reverse Central";
    // Start scanning to connect BACK to the client peripheral
    if (!discoveryAgent) {
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                this, &ChatServer::onDeviceDiscovered);

        QTimer::singleShot(1500, this, [this]() {
            discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        });
    }
}

void ChatServer::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (!(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)) {
        return;
    }

    // Try multiple matching strategies. Android often does NOT include
    // the device name or service UUIDs in advertisements — they may
    // only become available after connection and service discovery.
    bool nameMatch = info.name().contains("Gobblet", Qt::CaseInsensitive);
    bool uuidMatch = info.serviceUuids().contains(reverseServiceUuid);

    // Match by address (works on Android/Linux where remoteAddress() returns a real MAC)
    bool addressMatch = !connectedClientAddress.isNull()
                        && info.address() == connectedClientAddress;

    // Match by CoreBluetooth device UUID (works on macOS/iOS)
    bool deviceUuidMatch = !connectedClientUuid.isNull()
                           && info.deviceUuid() == connectedClientUuid;

    if (!nameMatch && !uuidMatch && !addressMatch && !deviceUuidMatch) {
        return;
    }

    qDebug() << "[Server-Central] Found peer peripheral:" << info.name()
             << "address:" << info.address().toString()
             << "(matched by:"
             << (nameMatch ? "name" : "")
             << (uuidMatch ? "uuid" : "")
             << (addressMatch ? "address" : "")
             << (deviceUuidMatch ? "deviceUuid" : "")
             << ")";

    // Connect as CENTRAL to the client peripheral
    discoveryAgent->stop();
    discoveryAgent->deleteLater();
    discoveryAgent = nullptr;
    centralController = QLowEnergyController::createCentral(info, this);

    connect(centralController, &QLowEnergyController::connected, this, [this]() {
        qDebug() << "[Server-Central] Connected → discovering services";
        centralController->discoverServices();
    });

    connect(centralController, &QLowEnergyController::serviceDiscovered,
            this, &ChatServer::serviceDiscovered);

    centralController->connectToDevice();
}

void ChatServer::serviceDiscovered(const QBluetoothUuid &uuid)
{
    if (uuid != reverseServiceUuid) {
        qDebug() << "Not the correct service " << uuid;
        return;
    } else {
        qDebug() << "[Server-Central] Target service found";
    }

    centralService = centralController->createServiceObject(uuid, this);
    if (!centralService) {
        return;
    }

    connect(centralService, &QLowEnergyService::stateChanged,
            this, &ChatServer::serviceStateChanged);

    connect(centralService, &QLowEnergyService::characteristicChanged,
            this, &ChatServer::updateNotification);

    centralService->discoverDetails();
}

void ChatServer::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    if(newState != QLowEnergyService::RemoteServiceDiscovered) {
        return;
    }

    qDebug() << "[Server-Central] Service discovered → subscribing";

    // Subscribe to client TX notify
    centralTxChar = centralService->characteristic(reverseTxUuid);

    QLowEnergyDescriptor ccc =
        centralTxChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

    if (ccc.isValid()) {
        centralService->writeDescriptor(ccc, QByteArray::fromHex("0100"));
    }
}

void ChatServer::updateNotification(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << "[ChatServer] Notification:"
             << characteristic.uuid().toString()
             << "value:" << value.toHex();

    if (characteristic.uuid() != reverseTxUuid) {
        return;
    }

    const QString message = QString::fromUtf8(value);
    qDebug() << "[ChatServer] Message received:" << message;

    emit messageReceived("Remote", message);
}
