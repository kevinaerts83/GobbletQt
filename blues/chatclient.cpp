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

    qDebug() << "[ChatClient] Connecting to:" << deviceInfo.name();

    controller = QLowEnergyController::createCentral(deviceInfo, this);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatClient::controllerStateChanged);

    connect(controller, &QLowEnergyController::serviceDiscovered,
            this, &ChatClient::serviceDiscovered);

    connect(controller, &QLowEnergyController::discoveryFinished,
            this, &ChatClient::serviceScanFinished);

    connect(controller, &QLowEnergyController::errorOccurred,
            this, [this](QLowEnergyController::Error error) {
                emit socketErrorOccurred(QStringLiteral("Controller error: %1").arg(error));
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
    qDebug() << "[ChatClient] Service discovered:" << uuid.toString();

    if (uuid != serviceUuid)
        return;

    qDebug() << "[ChatClient] Target service found";

    service = controller->createServiceObject(uuid, this);
    if (!service) {
        emit socketErrorOccurred("Failed to create service object");
        return;
    }

    connect(service, &QLowEnergyService::stateChanged,
            this, &ChatClient::serviceStateChanged);

    connect(service, &QLowEnergyService::characteristicChanged,
            this, &ChatClient::characteristicChanged);

    connect(service, &QLowEnergyService::errorOccurred,
            this, [this](QLowEnergyService::ServiceError error) {
                emit socketErrorOccurred(QStringLiteral("Service error: %1").arg(error));
            });

    service->discoverDetails();
}

void ChatClient::serviceScanFinished()
{
    qDebug() << "[ChatClient] Service scan finished";
}

void ChatClient::serviceStateChanged(QLowEnergyService::ServiceState state)
{
    if (state != QLowEnergyService::RemoteServiceDiscovered)
        return;

    qDebug() << "[ChatClient] Service details discovered";

    rxChar = service->characteristic(rxUuid);
    txChar = service->characteristic(txUuid);

    if (!rxChar.isValid() || !txChar.isValid()) {
        emit socketErrorOccurred("RX or TX characteristic missing");
        return;
    }

    // Enable notifications on TX (server -> client)
    QLowEnergyDescriptor ccc =
        txChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

    if (ccc.isValid()) {
        service->writeDescriptor(ccc, QByteArray::fromHex("0100"));
        qDebug() << "[ChatClient] Notifications enabled";
    } else {
        qWarning() << "[ChatClient] CCC descriptor not found";
    }

    emit connected();
}

void ChatClient::characteristicChanged(const QLowEnergyCharacteristic &characteristic,
                                       const QByteArray &value)
{
    if (characteristic.uuid() != txUuid)
        return;

    const QString message = QString::fromUtf8(value);
    qDebug() << "[ChatClient] Received notification:" << message;

    emit messageReceived("Remote", message);
}

void ChatClient::sendMessage(const QString &message)
{
    if (!service || !rxChar.isValid()) {
        qWarning() << "[ChatClient] Cannot send message: RX invalid";
        return;
    }

    qDebug() << "[ChatClient] Sending message:" << message;

    service->writeCharacteristic(
        rxChar,
        message.toUtf8(),
        QLowEnergyService::WriteWithResponse
        );
}
