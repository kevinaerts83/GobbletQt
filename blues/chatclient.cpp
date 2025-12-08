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

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatClient::controllerStateChanged);
    connect(controller, &QLowEnergyController::serviceDiscovered,
        this, &::ChatClient::serviceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished,
            this, &ChatClient::serviceDetailsDiscovered);

    // Start connecting
    controller->connectToDevice();
    controller->discoverServices();
}

void ChatClient::controllerStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
    case QLowEnergyController::ConnectedState:
        qDebug() << "[ChatClient] Connected to device, discovering services";
        break;
    case QLowEnergyController::UnconnectedState:
        emit disconnected();
        cleanupController();
        break;
    default:
        break;
    }
}

void ChatClient::serviceDiscovered(const QBluetoothUuid &uuid)
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
            this, &ChatClient::updateNotification);

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

void ChatClient::serviceDetailsDiscovered()
{
    if (!service) return;
    if (service->state() != QLowEnergyService::RemoteServiceDiscovered) return;

    // locate characteristics
    const auto chars = service->characteristics();
    for (const auto &c : chars) {
        if (c.uuid() == txUuid) {
            txChar = c;
            // subscribe to notifications if supported
            if (txChar.properties() & QLowEnergyCharacteristic::Notify) {
                connect(service, &QLowEnergyService::characteristicChanged,
                        this, &ChatClient::updateNotification);
                // write CCC descriptor to enable notifications (core behavior)
                const auto descs = txChar.descriptors();
                for (const auto &d : descs) {
                    if (d.uuid() == QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration) {
                        service->writeDescriptor(d, QByteArray::fromHex("0100"));
                        break;
                    }
                }
            }
        } else if (c.uuid() == rxUuid) {
            rxChar = c; // we'll write to this to send data to the server
        }
    }

    if (service->state() == QLowEnergyService::RemoteServiceDiscovered) {
        emit connected();
    }
}

void ChatClient::updateNotification(const QLowEnergyCharacteristic &c, const QByteArray &value)
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
    service->writeCharacteristic(rxChar, message.toUtf8(), QLowEnergyService::WriteWithResponse);
}
