#include "chatclient.h"

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
    this->serviceUuid = serviceUuid;
    this->rxUuid = rxCharUuid;
    this->txUuid = txCharUuid;

    cleanupController();

    qDebug() << "Connecting to BLE device:" << deviceInfo.name();

    controller = QLowEnergyController::createCentral(deviceInfo, this);

    // Controller signals
    connect(controller, &QLowEnergyController::connected,
        this, &ChatClient::onControllerConnected);
    connect(controller, &QLowEnergyController::serviceDiscovered,
        this, &::ChatClient::onServiceDiscovered);

    connect(controller, &QLowEnergyController::discoveryFinished, this, [this]() {
        qDebug() << "BLE discovery finished";
    });
    connect(controller, &QLowEnergyController::errorOccurred, this, [this]() {
        qDebug() << "BLE an error occurred";
    });
    connect(controller, &QLowEnergyController::disconnected, this, [this]() {
        qDebug() << "BLE device disconnected.";
        emit disconnected();
    });


    // Start connecting
    controller->connectToDevice();
}

void ChatClient::disconnectClient()
{
    if (controller)
        controller->disconnectFromDevice();
}

void ChatClient::onControllerDisconnected()
{
    qDebug() << "Controller disconnected";
    emit disconnected();
    cleanupController();
}

void ChatClient::onControllerConnected()
{
    qDebug() << "Controller connected. Starting service discovery...";
    emit connected(controller->remoteName());
    controller->discoverServices();   // REQUIRED ON MACOS
}

void ChatClient::onServiceDiscovered(const QBluetoothUuid &uuid)
{
    qDebug() << "Service discovered:" << uuid;

    if (uuid != serviceUuid) {
        qDebug() << "Uuid doesn't match";
        return;
    }

    qDebug() << "Target BLE service discovered, creating service object...";

    service = controller->createServiceObject(serviceUuid, this);

    if (!service) {
        emit socketErrorOccurred("Failed to create BLE service object after discovery");
        return;
    }

    connect(service, &QLowEnergyService::stateChanged,
            this, &ChatClient::serviceStateChanged);

    connect(service, &QLowEnergyService::characteristicChanged,
            this, &ChatClient::characteristicChanged);

    service->discoverDetails();
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
