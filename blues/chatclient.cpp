#include "chatclient.h"

#include <QLowEnergyDescriptor>
#include <QDebug>

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
    if (service) {
        service->disconnect(this);
        service->deleteLater();
        service = nullptr;
    }

    if (controller) {
        controller->disconnect(this);
        controller->disconnectFromDevice();
        controller->deleteLater();
        controller = nullptr;
    }

    rxChar = QLowEnergyCharacteristic();
    txChar = QLowEnergyCharacteristic();
}

void ChatClient::startClient(const QBluetoothDeviceInfo &deviceInfo,
                             const QBluetoothUuid &serviceUuid,
                             const QBluetoothUuid &rxCharUuid,
                             const QBluetoothUuid &txCharUuid)
{
    cleanupController();

    this->serviceUuid = serviceUuid;
    this->rxUuid = rxCharUuid;
    this->txUuid = txCharUuid;

    qDebug() << "[ChatClient] Connecting to:" << deviceInfo.name()
             << deviceInfo.address().toString();

    controller = QLowEnergyController::createCentral(deviceInfo, this);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatClient::controllerStateChanged);

    connect(controller, &QLowEnergyController::serviceDiscovered,
            this, &ChatClient::serviceDiscovered);

    connect(controller, &QLowEnergyController::errorOccurred,
            this, [this](QLowEnergyController::Error error) {
                emit socketErrorOccurred(
                    QStringLiteral("Controller error: %1").arg(error));
            });

    controller->connectToDevice();
}

void ChatClient::controllerStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
    case QLowEnergyController::ConnectedState:
        qDebug() << "[ChatClient] Connected. Discovering services...";
        controller->discoverServices();
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

    service = controller->createServiceObject(uuid, this);
    if (!service) {
        emit socketErrorOccurred("Failed to create service object");
        return;
    }

    connect(service, &QLowEnergyService::stateChanged,
            this, &ChatClient::serviceStateChanged);

    connect(service, &QLowEnergyService::characteristicChanged,
            this, &ChatClient::updateNotification);

    connect(service, &QLowEnergyService::errorOccurred,
            this, [this](QLowEnergyService::ServiceError error) {
                emit socketErrorOccurred(
                    QStringLiteral("Service error: %1").arg(error));
            });

    qDebug() << "[ChatClient] Discovering service details...";
    service->discoverDetails();
}

void ChatClient::serviceScanFinished()
{
    qDebug() << "[ChatClient] Service scan finished";

#ifdef Q_OS_WIN
    if (serviceObjectCreated)
        return;

    serviceObjectCreated = true;

    service = controller->createServiceObject(serviceUuid, this);
    if (!service) {
        emit socketErrorOccurred("Failed to create service object");
        return;
    }

    connect(service, &QLowEnergyService::stateChanged,
            this, &ChatClient::serviceStateChanged);

    connect(service, &QLowEnergyService::characteristicChanged,
            this, &ChatClient::updateNotification);

    service->discoverDetails();
#endif
}


void ChatClient::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::RemoteServiceDiscovered)
        return;

    qDebug() << "[ChatClient] Service details discovered";
    qDebug() << "[ChatClient] Available characteristics:";

    for (const auto &c : service->characteristics()) {
        qDebug() << "  -" << c.uuid().toString()
        << "props:" << c.properties();
    }

    rxChar = service->characteristic(rxUuid); // client → server
    txChar = service->characteristic(txUuid); // server → client

    qDebug() << "[ChatClient] RX UUID expected:" << rxUuid.toString()
             << "found:" << rxChar.uuid().toString();
    qDebug() << "[ChatClient] TX UUID expected:" << txUuid.toString()
             << "found:" << txChar.uuid().toString();

    if (!rxChar.isValid() || !txChar.isValid()) {
        emit socketErrorOccurred("RX or TX characteristic missing");
        return;
    }

    // Enable notifications on TX
    QLowEnergyDescriptor ccc =
        txChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

    if (ccc.isValid()) {
        service->writeDescriptor(ccc, QByteArray::fromHex("0100"));
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
    if (!service || !rxChar.isValid()) {
        qWarning() << "[ChatClient] Cannot send message: RX invalid";
        return;
    }

    qDebug() << "[ChatClient] Sending message:" << message;

    // 🔑 iOS-compatible write (REQUIRED)
    service->writeCharacteristic(
        rxChar,
        message.toUtf8(),
        QLowEnergyService::WriteWithoutResponse
        );
}
