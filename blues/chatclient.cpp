#include "chatclient.h"

#include <QLowEnergyDescriptor>
#include <QDebug>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
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

    central->connectToDevice();
}

void ChatClient::startClientPeripheral()
{
    if (peripheral)
        return;

    qDebug() << "[ChatClient] Starting client-side peripheral";

    peripheral = QLowEnergyController::createPeripheral(this);

    QLowEnergyCharacteristicData txData;
    txData.setUuid(rxUuid); // IMPORTANT: reuse RX UUID (server listens here)
    txData.setProperties(QLowEnergyCharacteristic::Notify);
    txData.setValue(QByteArray());

    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(serviceUuid);
    serviceData.addCharacteristic(txData);

    peripheralService = peripheral->addService(serviceData);

    txChar = peripheralService->characteristic(rxUuid);

    QLowEnergyAdvertisingData adv;
    adv.setLocalName("Gobblet Client");
    adv.setServices({ serviceUuid });

    peripheral->startAdvertising(QLowEnergyAdvertisingParameters(), adv, adv);

    qDebug() << "[ChatClient] Client peripheral advertising";
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
                qDebug() << "[ChatClient] Write confirmed:"
                         << c.uuid() << v;
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
    if (newState != QLowEnergyService::RemoteServiceDiscovered) {
        return;
    }

    startClientPeripheral();

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
    if (rxChar.isValid()) {
        QLowEnergyDescriptor rxCcc = rxChar.descriptor(
            QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);

        if (rxCcc.isValid()) {
            qDebug() << "[ChatClient] Enabling notifications on RX too (for write path)";
            centralService->writeDescriptor(rxCcc, QByteArray::fromHex("0100"));
        } else {
            qWarning() << "[ChatClient] TX CCC descriptor missing (notifications may fail)";
        }
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
    if (!peripheralService || !txChar.isValid()) {
        qWarning() << "[ChatClient] Client TX not ready";
        return;
    }

    // Check if RX supports write or write without response
    auto props = txChar.properties();

    qDebug() << "[ChatClient] Notifying to TX:"
             << message
             << "uuid:" << txChar.uuid()
             << "props:" << props
             << "valid:" << txChar.isValid()
             << "len:" << message.toUtf8().size();

    qDebug() << "[ChatClient] Notify → server:" << message;

    peripheralService->writeCharacteristic(txChar, message.toUtf8(), QLowEnergyService::WriteWithoutResponse);
}

