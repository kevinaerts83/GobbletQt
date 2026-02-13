#include "chatserver.h"

#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QDebug>
#include <qlowenergydescriptordata.h>
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
                             const QBluetoothUuid &txCharUuid)
{
    if (controller) {
        qWarning() << "BLE ChatServer already running";
        return;
    }

    qDebug() << "Starting BLE ChatServer (peripheral)...";

    rxUuid = rxCharUuid;
    txUuid = txCharUuid;

    controller = QLowEnergyController::createPeripheral(this);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatServer::onConnectionStateChanged);

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

    connect(service, &QLowEnergyService::characteristicWritten,
            this, &ChatServer::onCharacteristicWritten);

    connect(service, &QLowEnergyService::characteristicWritten,
            this, [](const QLowEnergyCharacteristic &c, const QByteArray &v) {
                qDebug() << "[ChatServer] Write CONFIRMED by stack:"
                         << c.uuid()
                         << "value:" << v;
            });

    // Advertising
    QLowEnergyAdvertisingParameters params;
    params.setMode(QLowEnergyAdvertisingParameters::AdvInd);
    params.setInterval(100, 200);

    QLowEnergyAdvertisingData adv;
    adv.setLocalName("Gobblet server");
    adv.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);

    QLowEnergyAdvertisingData scanResp;
    scanResp.setServices({ serviceUuid });

    controller->startAdvertising(params, adv, scanResp);

    qDebug() << "Advertising BLE service:" << serviceUuid.toString();
}

void ChatServer::stopServer()
{
    if (!controller)
        return;

    qDebug() << "Stopping BLE ChatServer";

    controller->stopAdvertising();
    controller->deleteLater();

    controller = nullptr;
    service = nullptr;
    rxChar = QLowEnergyCharacteristic();
    txChar = QLowEnergyCharacteristic();
}

void ChatServer::onCharacteristicWritten(const QLowEnergyCharacteristic &c,
                                         const QByteArray &value)
{
    if (c.uuid() != rxUuid) {
        qDebug() << "This is OUR OWN TX Notify — ignore it"
                 << "uuid:" << c.uuid()
                 << "expected rxUuid:" << rxUuid;
        return;
    }

    qDebug() << "[Server] NOTIFY EVENT"
             << "uuid:" << c.uuid()
             << "valid:" << c.isValid()
             << "props:" << c.properties()
             << "value Hex:" << value.toHex()
             << "value ascii:" << QString::fromUtf8(value);

    if (c.uuid() == rxUuid) {
        qDebug() << "[Server] RX RECEIVED";
        emit messageReceived("Client", QString::fromUtf8(value));
    } else {
        qDebug() << "[Server] NOT RX";
    }
}

void ChatServer::sendMessage(const QString &message)
{
    if (!service || !txChar.isValid()) {
        qWarning() << "Cannot send message: TX characteristic invalid";
        return;
    }

    qDebug() << "Sending BLE notification:" << message;

    // THIS IS THE ONLY CORRECT WAY (iOS/macOS compatible)
    service->writeCharacteristic(
        txChar,
        message.toUtf8(),
        QLowEnergyService::WriteWithoutResponse
        );
}

void ChatServer::onConnectionStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
    case QLowEnergyController::ConnectedState:
        qDebug() << "BLE central connected to our peripheral";
        emit clientConnected("BLE Central");

        // Start scanning to connect BACK to the client peripheral
        if (!discoveryAgent) {
            discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, [this](const QBluetoothDeviceInfo &info) {

        if (!(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration))
            return;

        if (!info.name().contains("Gobblet", Qt::CaseInsensitive))
            return;

        qDebug() << "[Server-Central] Found peer peripheral:" << info.name();

        discoveryAgent->stop();

        // Connect as CENTRAL to the client peripheral
        centralController = QLowEnergyController::createCentral(info, this);

        connect(centralController, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "[Server-Central] Connected → discovering services";
            centralController->discoverServices();
        });

        connect(centralController, &QLowEnergyController::serviceDiscovered,
            this, [this](const QBluetoothUuid &uuid) {

        if (uuid != service->serviceUuid())
            return;

        qDebug() << "[Server-Central] Target service found";

        centralService = centralController->createServiceObject(uuid, this);
        if (!centralService)
            return;

        connect(centralService, &QLowEnergyService::stateChanged,
            this, [this](QLowEnergyService::ServiceState s) {

        if (s != QLowEnergyService::RemoteServiceDiscovered)
            return;

        qDebug() << "[Server-Central] Service discovered → subscribing";

        // Subscribe to client TX notify
        centralTxChar = centralService->characteristic(txUuid);

        auto ccc = centralTxChar.descriptor(
            QBluetoothUuid(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration));

        if (ccc.isValid())
            centralService->writeDescriptor(ccc, QByteArray::fromHex("0100"));

        connect(centralService, &QLowEnergyService::characteristicChanged,
            this, [this](const QLowEnergyCharacteristic &c, const QByteArray &v) {

                if (c.uuid() != txUuid)
                    return;

                const QString msg = QString::fromUtf8(v);
                    qDebug() << "[Server] Received from client (notify):" << msg;

                emit messageReceived("Client", msg);
            });
        });

        centralService->discoverDetails();
        });

        centralController->connectToDevice();
        });

        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        }
    break;

    case QLowEnergyController::UnconnectedState:
        qDebug() << "BLE central disconnected";
        emit clientDisconnected("BLE Central");
    break;

    default:
    break;
    }
}
