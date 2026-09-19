#include "chatclient.h"

#include <QLowEnergyDescriptor>
#include <QTimer>
#include <QDebug>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
#include <QtBluetooth/qlowenergydescriptordata.h>
#include <QtBluetooth/qlowenergyservicedata.h>

ChatClient::ChatClient(QObject *parent)
    : QObject(parent)
{
}

ChatClient::~ChatClient()
{
    cleanupController();
}

void ChatClient::cleanupController()
{
    if (centralService) {
        centralService->disconnect(this);
        centralService->deleteLater();
        centralService = nullptr;
    }

    if (central) {
        central->disconnect(this);
        central->disconnectFromDevice();
        central->deleteLater();
        central = nullptr;
    }

    if (peripheralService) {
        peripheralService->disconnect(this);
        peripheralService->deleteLater();
        peripheralService = nullptr;
    }

    if (peripheral) {
        peripheral->disconnect(this);
        peripheral->disconnectFromDevice();
        peripheral->deleteLater();
        peripheral = nullptr;
    }

    serviceObjectCreated = false;
    rxChar = QLowEnergyCharacteristic();
    txChar = QLowEnergyCharacteristic();
}

void ChatClient::startClient(const QBluetoothDeviceInfo &deviceInfo,
                             const QBluetoothUuid &serviceUuid,
                             const QBluetoothUuid &rxCharUuid,
                             const QBluetoothUuid &txCharUuid,
                             const QBluetoothUuid &reverseServiceUuid,
                             const QBluetoothUuid &reverseRxCharUuid,
                             const QBluetoothUuid &reverseTxCharUuid)
{
    cleanupController();

    this->serviceUuid = serviceUuid;
    this->rxUuid = rxCharUuid;
    this->txUuid = txCharUuid;

    this->reverseServiceUuid = reverseServiceUuid;
    this->reverseRxUuid = reverseRxCharUuid;
    this->reverseTxUuid = reverseTxCharUuid;

    qDebug() << "[ChatClient] Connecting to:" << deviceInfo.name()
             << deviceInfo.address().toString();

    central = QLowEnergyController::createCentral(deviceInfo, this);

    connect(central, &QLowEnergyController::stateChanged,
            this, &ChatClient::controllerStateChanged);

    connect(central, &QLowEnergyController::serviceDiscovered,
            this, &ChatClient::serviceDiscovered);

    connect(central, &QLowEnergyController::errorOccurred,
            this, [this](QLowEnergyController::Error error) {
                emit socketErrorOccurred(
                    QStringLiteral("Controller error: %1").arg(error));
            });

    connect(central, &QLowEnergyController::discoveryFinished,
            this, &ChatClient::serviceScanFinished);

    central->connectToDevice();
}

void ChatClient::startClientPeripheral()
{
    if (peripheral)
        return;

    qDebug() << "[ChatClient] Starting client-side peripheral";

    peripheral = QLowEnergyController::createPeripheral(this);

    QLowEnergyCharacteristicData txData;
    txData.setUuid(reverseTxUuid);
    txData.setProperties(QLowEnergyCharacteristic::Notify);
    txData.setValue(QByteArray());

    QLowEnergyDescriptorData ccc(
        QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration,
        QByteArray(2, 0x00)
        );

    txData.addDescriptor(ccc);

    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(reverseServiceUuid);
    serviceData.addCharacteristic(txData);

    peripheralService = peripheral->addService(serviceData);

    reverseTxChar = peripheralService->characteristic(reverseTxUuid);

    // Advertising
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(
        QLowEnergyAdvertisingData::DiscoverabilityGeneral
        );
    advertisingData.setServices({ reverseServiceUuid });

    // Keep advertising packet small!
    QLowEnergyAdvertisingData scanResponseData;
    scanResponseData.setLocalName("Gobblet C");

    peripheral->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData, scanResponseData);

    qDebug() << "[ChatClient] Client peripheral advertising" << reverseServiceUuid.toString();
}


void ChatClient::controllerStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
    case QLowEnergyController::ConnectedState:
        qDebug() << "[ChatClient] Connected. Discovering services...";
        central->discoverServices();
        break;

    case QLowEnergyController::UnconnectedState:
        qDebug() << "[ChatClient] Disconnected";
        emit disconnected();
        cleanupController();
        break;

    default:
        break;
    }
}

void ChatClient::serviceDiscovered(const QBluetoothUuid &uuid)
{
    qDebug() << "[ChatClient] Service discovered:" << uuid;

    if (uuid != serviceUuid)
        return;

    if (serviceObjectCreated)
        return;

    qDebug() << "[ChatClient] Target service found (creating service object)";

    serviceObjectCreated = true;

    centralService = central->createServiceObject(uuid, this);
    if (!centralService) {
        emit socketErrorOccurred("Failed to create service object");
        return;
    }

    connect(centralService, &QLowEnergyService::stateChanged,
            this, &ChatClient::serviceStateChanged);

    connect(centralService, &QLowEnergyService::characteristicChanged,
            this, &ChatClient::updateNotification);

    connect(centralService, &QLowEnergyService::characteristicChanged,
            this, [](const QLowEnergyCharacteristic &c, const QByteArray &v) {
                qDebug() << "[ChatClient] Notification received:" << c.uuid() << v;
            });

    connect(centralService, &QLowEnergyService::errorOccurred,
            this, [this](QLowEnergyService::ServiceError error) {
                emit socketErrorOccurred(
                    QStringLiteral("Service error: %1").arg(error));
            });

    qDebug() << "[ChatClient] Discovering service details...";
    centralService->discoverDetails();
}

void ChatClient::serviceScanFinished()
{
    qDebug() << "[ChatClient] Service scan finished";

#ifdef Q_OS_WIN
    if (serviceObjectCreated)
        return;

    serviceObjectCreated = true;

    centralService = central->createServiceObject(serviceUuid, this);
    if (!centralService) {
        emit socketErrorOccurred("Failed to create service object");
        return;
    }

    connect(centralService, &QLowEnergyService::stateChanged,
            this, &ChatClient::serviceStateChanged);

    connect(centralService, &QLowEnergyService::characteristicChanged,
            this, &ChatClient::updateNotification);

    centralService->discoverDetails();
#endif
}

void ChatClient::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::RemoteServiceDiscovered) {
        return;
    }

    // The reverse peripheral (client-side GATT server used to notify the server)
    // is no longer needed: the client now writes moves directly to the server's
    // forward RX characteristic. Advertising a second peripheral while acting as
    // a central was unreliable on Android, so it is intentionally disabled.
    // QTimer::singleShot(1500, this, &ChatClient::startClientPeripheral);

    qDebug() << "[ChatClient] Service details discovered";
    qDebug() << "[ChatClient] Available characteristics:";

    for (const auto &c : centralService->characteristics()) {
        qDebug() << "  -" << c.uuid().toString()
        << "props:" << c.properties();
    }

    rxChar = centralService->characteristic(rxUuid); // client → server
    txChar = centralService->characteristic(txUuid); // server → client

    qDebug() << "[ChatClient] RX UUID expected:" << rxUuid.toString()
             << "found:" << rxChar.uuid().toString();
    qDebug() << "[ChatClient] TX UUID expected:" << txUuid.toString()
             << "found:" << txChar.uuid().toString();

    // After discovering characteristics
    if (!rxChar.isValid() || !txChar.isValid()) {
        emit socketErrorOccurred("RX or TX characteristic missing");
        return;
    }

    // Enable notifications on TX
    QLowEnergyDescriptor ccc =
        txChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

    if (ccc.isValid()) {
        centralService->writeDescriptor(ccc, QByteArray::fromHex("0100"));
        qDebug() << "[ChatClient] TX notifications enabled";
    } else {
        qWarning() << "[ChatClient] TX CCC descriptor missing (notifications may fail)";
    }

    qDebug() << "[ChatClient] BLE ChatClient ready";
    emit connected();
}

void ChatClient::updateNotification(const QLowEnergyCharacteristic &characteristic,
                                    const QByteArray &value)
{
    qDebug() << "[ChatClient] Notification:"
             << characteristic.uuid().toString()
             << "value:" << value.toHex();

    if (characteristic.uuid() != txUuid)
        return;

    const QString message = QString::fromUtf8(value);
    qDebug() << "[ChatClient] Message received:" << message;

    emit messageReceived("Remote", message);
}

void ChatClient::sendMessage(const QString &message)
{
    // Client → Server uses the FORWARD GATT link that is already established:
    // the client is a central connected to the server's peripheral, and the
    // server exposes a writable RX characteristic (Write | WriteNoResponse).
    //
    // Writing to that characteristic works reliably on both iOS and Android.
    // The previous approach (notifying over the client's own peripheral via a
    // reverse channel) only worked on iOS: on Android the server's reverse
    // central could not reliably rediscover/reconnect to the client peripheral
    // (random advertising address, name only in scan response), so client moves
    // never reached the server.
    if (!centralService || !rxChar.isValid()) {
        qWarning() << "[ChatClient] Cannot send: RX characteristic invalid";
        return;
    }

    const QByteArray payload = message.toUtf8();
    const auto props = rxChar.properties();

    // Prefer a reliable, confirmed write (WriteWithResponse). Game moves are
    // infrequent and small, so latency is irrelevant while delivery guarantees
    // matter. WriteWithoutResponse gives no confirmation and is capped at ~20
    // bytes, so only fall back to it if the RX characteristic does NOT support
    // a plain Write. This behaves consistently on both macOS/iOS and Android.
    const bool supportsWrite = props & QLowEnergyCharacteristic::Write;
    const auto mode = supportsWrite
                          ? QLowEnergyService::WriteWithResponse
                          : QLowEnergyService::WriteWithoutResponse;

    qDebug() << "[ChatClient] Write → server RX:" << message
             << "uuid:" << rxChar.uuid().toString()
             << "mode:" << (mode == QLowEnergyService::WriteWithoutResponse
                                ? "WriteWithoutResponse" : "WriteWithResponse");

    centralService->writeCharacteristic(rxChar, payload, mode);
}

