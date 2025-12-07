#include "chatserver.h"

#include <QLowEnergyCharacteristic>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyDescriptorData>
#include <QLowEnergyServiceData>

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
    qDebug() << "Starting BLE ChatServer (peripheral)...";

    this->rxUuid = rxCharUuid;
    this->txUuid = txCharUuid;

    controller = QLowEnergyController::createPeripheral(this);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatServer::onConnectionStateChanged);

    // === Define characteristics ===
    QLowEnergyCharacteristicData rxData;
    rxData.setUuid(rxCharUuid);
    rxData.setProperties(QLowEnergyCharacteristic::Write |
                         QLowEnergyCharacteristic::WriteNoResponse);
    rxData.setValue(QByteArray());

    QLowEnergyCharacteristicData txData;
    txData.setUuid(txCharUuid);
    txData.setProperties(QLowEnergyCharacteristic::Notify | QLowEnergyCharacteristic::Read);
    txData.setValue(QByteArray());
    //QLowEnergyDescriptorData ccc(
    //    QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration,
    //    QByteArray(2, 0));
    //txData.addDescriptor(ccc);

    // === Define service ===
    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(serviceUuid);
    serviceData.addCharacteristic(rxData);
    serviceData.addCharacteristic(txData);

    service = controller->addService(serviceData);

    if (!service) {
        emit serverError("Failed to add service");
        return;
    }

    connect(service, &QLowEnergyService::characteristicWritten,
            this, &ChatServer::onCharacteristicWritten);

    // === Advertising ===
    QLowEnergyAdvertisingData adv;
    adv.setLocalName("BLE ChatServer");
    adv.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    adv.setServices({serviceUuid});
    controller->startAdvertising(QLowEnergyAdvertisingParameters(), adv, adv);

    qDebug() << "BLE ChatServer advertising service" << serviceUuid.toString();
}

void ChatServer::stopServer()
{
    if (controller) {
        controller->stopAdvertising();
        controller->disconnectFromDevice();
        controller->deleteLater();
        controller = nullptr;
        service = nullptr;
    }
    qDebug() << "BLE ChatServer stopped.";
}

void ChatServer::onCharacteristicWritten(const QLowEnergyCharacteristic &ch, const QByteArray &value)
{
    if (ch.uuid() == rxUuid) {
        QString message = QString::fromUtf8(value);
        qDebug() << "Received from client:" << message;
        emit messageReceived("Client", message);
    }
}

void ChatServer::sendMessage(const QString &message)
{
    if (!service) {
        qWarning() << "Cannot send message: service invalid";
        return;
    }

    auto chars = service->characteristics();
    for (const auto &c : std::as_const(chars)) {
        if (c.uuid() == txUuid) {

            if (!(c.properties() & QLowEnergyCharacteristic::Notify)) {
                qWarning() << "TX characteristic does not support Notify";
                return;
            }

            qDebug() << "Sending notification:" << message;

            service->writeCharacteristic(c, message.toUtf8(), QLowEnergyService::WriteWithResponse);
        }
    }
}

void ChatServer::onConnectionStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
    case QLowEnergyController::UnconnectedState:
        emit clientDisconnected("BLE Client");
        break;
    case QLowEnergyController::ConnectedState:
        emit clientConnected("BLE Client");
        break;
    default:
        break;
    }
}
