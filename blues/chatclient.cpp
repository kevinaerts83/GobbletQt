#include "chatclient.h"

ChatClient::ChatClient(QObject *parent)
    : QObject(parent)
{
}

void ChatClient::startClient(const QBluetoothDeviceInfo &deviceInfo,
                             const QBluetoothUuid &serviceUuid,
                             const QBluetoothUuid &rxCharUuid,
                             const QBluetoothUuid &txCharUuid)
{
    this->serviceUuid = serviceUuid;
    this->rxUuid = rxCharUuid;
    this->txUuid = txCharUuid;

    qDebug() << "Connecting to BLE device:" << deviceInfo.name();

    controller = QLowEnergyController::createCentral(deviceInfo, this);

    connect(controller, &QLowEnergyController::connected, this, [this, deviceInfo]() {
        qDebug() << "Connected to device. Discovering services...";
        emit connected(deviceInfo.name());
        controller->discoverServices();
    });

    connect(controller, &QLowEnergyController::discoveryFinished, this, [this, &serviceUuid]() {
        qDebug() << "Service discovery finished.";
        if (!controller->services().contains(serviceUuid)) {
            emit socketErrorOccurred("Target BLE service not found on device");
            controller->disconnectFromDevice();
            return;
        }

        qDebug() << "Connecting to BLE service" << serviceUuid.toString();
        service = controller->createServiceObject(serviceUuid, this);
        if (!service) {
            emit socketErrorOccurred("Failed to create BLE service object");
            return;
        }

        connect(service, &QLowEnergyService::stateChanged, this, &ChatClient::serviceStateChanged);
        connect(service, &QLowEnergyService::characteristicChanged, this, &ChatClient::characteristicChanged);
        service->discoverDetails();
    });

    connect(controller, &QLowEnergyController::errorOccurred, this, &ChatClient::controllerErrorOccurred);
    connect(controller, &QLowEnergyController::disconnected, this, [this]() {
        qDebug() << "BLE device disconnected.";
        emit disconnected();
    });

    controller->connectToDevice();
}

void ChatClient::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    if (newState == QLowEnergyService::RemoteServiceDiscovered) {
        qDebug() << "BLE service discovered successfully.";

        rxChar = service->characteristic(rxUuid);
        txChar = service->characteristic(txUuid);

        if (!rxChar.isValid() || !txChar.isValid()) {
            emit socketErrorOccurred("Missing RX/TX characteristics in service");
            return;
        }

        // Enable notifications for RX
        QLowEnergyDescriptor notificationDesc = rxChar.descriptor(
            QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
        if (notificationDesc.isValid()) {
            service->writeDescriptor(notificationDesc, QByteArray::fromHex("0100"));
        }

        qDebug() << "BLE ChatClient ready. RX/TX characteristics configured.";
    }
}

void ChatClient::characteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    if (c.uuid() == rxUuid) {
        QString message = QString::fromUtf8(value);
        qDebug() << "Received message:" << message;
        emit messageReceived("Remote", message);
    }
}

void ChatClient::sendMessage(const QString &message)
{
    if (!service || !txChar.isValid()) {
        qWarning() << "Cannot send message: BLE service or TX characteristic invalid";
        return;
    }

    QByteArray data = message.toUtf8();
    service->writeCharacteristic(txChar, data, QLowEnergyService::WriteWithoutResponse);
    qDebug() << "Sent message:" << message;
}

void ChatClient::controllerErrorOccurred(QLowEnergyController::Error error)
{
    QString err;
    switch (error) {
    case QLowEnergyController::ConnectionError: err = "Connection error"; break;
    case QLowEnergyController::RemoteHostClosedError: err = "Remote closed connection"; break;
    default: err = QString("Error %1").arg(error); break;
    }
    qWarning() << "Controller error:" << err;
    emit socketErrorOccurred(err);
}
