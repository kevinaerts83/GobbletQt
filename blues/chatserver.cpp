#include "chatserver.h"

#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QDebug>
#include <qlowenergydescriptordata.h>

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

    // 🔑 CACHE CHARACTERISTICS (CRITICAL)
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
        // This is OUR OWN TX write — ignore it
        return;
    }

    const QString msg = QString::fromUtf8(value);
    qDebug() << "[Server] Received from client:" << msg;
    emit messageReceived("Client", msg);
}

void ChatServer::sendMessage(const QString &message)
{
    if (!service || !txChar.isValid()) {
        qWarning() << "Cannot send message: TX characteristic invalid";
        return;
    }

    qDebug() << "Sending BLE notification:" << message;

    // 🔑 THIS IS THE ONLY CORRECT WAY (iOS/macOS compatible)
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
