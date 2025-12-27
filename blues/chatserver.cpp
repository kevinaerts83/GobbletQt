#include "chatserver.h"

#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QDebug>
#include <QLowEnergyDescriptorData>

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

    // === Create peripheral controller ===
    controller = QLowEnergyController::createPeripheral(this);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &ChatServer::onConnectionStateChanged);

    connect(controller, &QLowEnergyController::errorOccurred,
            this, [this](QLowEnergyController::Error error) {
                emit serverError(QStringLiteral("Controller error: %1").arg(error));
            });

    // === RX characteristic (client -> server) ===
    QLowEnergyCharacteristicData rxData;
    rxData.setUuid(rxUuid);
    rxData.setProperties(QLowEnergyCharacteristic::Write |
                         QLowEnergyCharacteristic::WriteNoResponse);
    rxData.setValue(QByteArray());

    // === TX characteristic (server -> client) ===
    QLowEnergyCharacteristicData txData;
    txData.setUuid(txUuid);
    txData.setProperties(QLowEnergyCharacteristic::Notify |
                         QLowEnergyCharacteristic::Read);
    txData.setValue(QByteArray());

    QLowEnergyDescriptorData ccc(
        QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration,
        QByteArray(2, 0x00)
        );
    txData.addDescriptor(ccc);

    // DO NOT add CCC descriptor manually (macOS breaks)

    // === Service definition ===
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

    connect(service, &QLowEnergyService::stateChanged,
            this, [](QLowEnergyService::ServiceState state) {
                qDebug() << "Service state changed:" << state;
            });

    connect(service, &QLowEnergyService::characteristicWritten,
            this, &ChatServer::onCharacteristicWritten);

    // === Advertising parameters (CRITICAL for Windows/macOS) ===
    QLowEnergyAdvertisingParameters params;
    params.setMode(QLowEnergyAdvertisingParameters::AdvInd);
    params.setInterval(100, 200); // milliseconds

    // Advertising packet (name)
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setLocalName("BLE ChatServer");
    advertisingData.setDiscoverability(
        QLowEnergyAdvertisingData::DiscoverabilityGeneral);

    // Scan response (service UUID)
    QLowEnergyAdvertisingData scanResponse;
    scanResponse.setServices({ serviceUuid });

    controller->startAdvertising(params, advertisingData, scanResponse);

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
}

void ChatServer::onCharacteristicWritten(const QLowEnergyCharacteristic &characteristic,
                                         const QByteArray &value)
{
    if (!characteristic.isValid())
        return;

    if (characteristic.uuid() == rxUuid) {
        const QString message = QString::fromUtf8(value);
        qDebug() << "Received from client:" << message;
        emit messageReceived("Client", message);
    }
}

void ChatServer::sendMessage(const QString &message)
{
    if (!service) {
        qWarning() << "Cannot send message: service not available";
        return;
    }

    const auto characteristics = service->characteristics();
    for (const auto &c : characteristics) {
        if (c.uuid() == txUuid) {

            if (!c.isValid()) {
                qWarning() << "TX characteristic invalid";
                return;
            }

            if (!(c.properties() & QLowEnergyCharacteristic::Notify)) {
                qWarning() << "TX characteristic does not support Notify";
                return;
            }

            qDebug() << "Sending BLE notification:" << message << "props:" << c.properties();

            // WriteWithResponse triggers Notify update safely on all platforms
            service->writeCharacteristic(
                c,
                message.toUtf8(),
                QLowEnergyService::WriteWithResponse
                );

            return;
        }
    }

    qWarning() << "TX characteristic not found";
}

void ChatServer::onConnectionStateChanged(QLowEnergyController::ControllerState state)
{
    switch (state) {
    case QLowEnergyController::ConnectedState:
        qDebug() << "BLE central connected";
        emit clientConnected("BLE Central");
        break;

    case QLowEnergyController::UnconnectedState:
        qDebug() << "BLE central disconnected";
        emit clientDisconnected("BLE Central");
        break;

    default:
        break;
    }
}
