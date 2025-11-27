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

void ChatServer::startServer()
{
    qDebug() << "Starting BLE ChatServer (peripheral)...";

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
    txData.setProperties(QLowEnergyCharacteristic::Notify);
    txData.setValue(QByteArray());

    // Notification descriptor (mandatory for notify)
    QLowEnergyDescriptorData ccc(
        QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration,
        QByteArray(2, 0));
    txData.addDescriptor(ccc);

    // === Define service ===
    QLowEnergyServiceData serviceData;
    serviceData.setUuid(serviceUuid);
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.addCharacteristic(rxData);
    serviceData.addCharacteristic(txData);

    service = controller->addService(serviceData);

    connect(service, &QLowEnergyService::characteristicWritten,
            this, &ChatServer::onCharacteristicWritten);


    // === Advertising ===
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setLocalName("BLE ChatServer");
    advertisingData.setIncludePowerLevel(true);
    advertisingData.setServices({serviceUuid});

    controller->startAdvertising(QLowEnergyAdvertisingParameters(),
                                 advertisingData, advertisingData);

    qDebug() << "BLE ChatServer advertising service" << serviceUuid.toString();
}

void ChatServer::stopServer()
{
    if (controller) {
        controller->stopAdvertising();
        controller->disconnectFromDevice();
        controller->deleteLater();
        controller = nullptr;
    }
    qDebug() << "BLE ChatServer stopped.";
}

void ChatServer::onCharacteristicWritten(const QLowEnergyCharacteristic &ch, const QByteArray &value)
{
    if (ch.uuid() == rxCharUuid) {
        QString message = QString::fromUtf8(value);
        qDebug() << "Received from client:" << message;
        emit messageReceived("Client", message);
    }
}

void ChatServer::sendMessage(const QString &message)
{
    if (!service) return;

    const auto chars = service->characteristics();
    for (const auto &c : chars) {
        if (c.uuid() == txCharUuid) {
            qDebug() << "Sending message to clients:" << message;
            service->writeCharacteristic(c, message.toUtf8());
        }
    }
}

void ChatServer::serverError(const QString &message)
{
    qDebug() << "Server error:" << message;
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
