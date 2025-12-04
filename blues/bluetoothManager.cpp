#include "bluetoothManager.h"
#include <QDebug>
#include <QCoreApplication>
#include <QBluetoothPermission>

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent)
{

}

BluetoothManager::~BluetoothManager()
{
    if (discoveryAgent) {
        discoveryAgent->stop();
        discoveryAgent->deleteLater();
        discoveryAgent = nullptr;
    }
}


void BluetoothManager::startServer()
{

#if QT_CONFIG(permissions)
    QBluetoothPermission btPerm;

    auto status = qApp->checkPermission(btPerm);

    if (status == Qt::PermissionStatus::Undetermined) {

        qApp->requestPermission(btPerm, this, [this](const QPermission &p) {
            if (p.status() == Qt::PermissionStatus::Granted) {
                this->startServer();  // retry
            } else {
                emit onServerError("Bluetooth permission denied");
            }
        });
        return;
    }

    if (status == Qt::PermissionStatus::Denied) {
        emit onServerError("Bluetooth permission denied");
        return;
    }
#endif

    qDebug() << "Initializing BLE ChatServer...";

    server = new ChatServer(this);

    connect(server, &ChatServer::messageReceived,
            this, &BluetoothManager::serverMessage);
    connect(server, &ChatServer::clientConnected,
            this, &BluetoothManager::clientConnected);
    connect(server, &ChatServer::clientDisconnected,
            this, &BluetoothManager::clientDisconnected);
    //connect(server, &ChatServer::serverError,
    //        this, &BluetoothManager::onServerError);
    //connect(this, &BluetoothManager::sendMessage,
    //        server, &ChatServer::sendMessage);


    server->startServer();

    setServerName("Gobblet Online");
}

void BluetoothManager::stopServer()
{
    if (!server)
        return;
    server->stopServer();
    server->deleteLater();
    server = nullptr;
}

void BluetoothManager::initClient()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // Optional: give macOS enough time
    discoveryAgent->setLowEnergyDiscoveryTimeout(10000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
        this, &BluetoothManager::onDeviceDiscovered);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothManager::discoveryFinished);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &BluetoothManager::discoveryFinished); // macOS workaround
}

void BluetoothManager::startDiscovery()
{
    if (discoveryAgent == nullptr) {
        initClient();
    }

    qDebug() << "Starting BLE device discovery...";
    if (foundDevices.count() > 5) {
        foundDevices.clear();
    }
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BluetoothManager::stopDiscovery()
{
    if (discoveryAgent) discoveryAgent->stop();
}

void BluetoothManager::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (!(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration))
        return;

    qDebug() << "Found BLE device:" << info.name() << info.address().toString();
    foundDevices.append(info);

    stopDiscovery();
}

void BluetoothManager::discoveryFinished()
{
    qDebug() << "BLE device discovery finished.";
    if (foundDevices.isEmpty())
        qDebug() << "No BLE devices found.";
}

void BluetoothManager::connectWithAddress(const QString &address)
{
    for (const QBluetoothDeviceInfo &d : std::as_const(foundDevices)) {
        if (d.address().toString().contains(address, Qt::CaseInsensitive)) {
            stopDiscovery();
            connectToDevice(d);
            break;
        }
    }
}

void BluetoothManager::connectToDevice(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Connecting to device:" << device.name();

    if (client) {
        client->disconnect(this);   // disconnect all signals
        client->deleteLater();
        client = nullptr;
    }
    client = new ChatClient(this);

    connect(client, &ChatClient::messageReceived, this, &BluetoothManager::clientMessage);
    connect(client, &ChatClient::disconnected, this, &BluetoothManager::clientDisconnected);
    connect(client, &ChatClient::connected, this, &BluetoothManager::clientConnected);
    connect(client, &ChatClient::socketErrorOccurred, this, &BluetoothManager::reactOnSocketError);
    connect(this, &BluetoothManager::sendMessage, client, &ChatClient::sendMessage);

    // Start BLE connection
    client->startClient(device, serviceUuid, rxCharUuid, txCharUuid);

    setClientName("Connected");
}

QVariantList BluetoothManager::getDevices() {

    startDiscovery();

    QVariantList devices;
    for (const QBluetoothDeviceInfo &d : std::as_const(foundDevices)) {
        QVariantMap device;
        // Example: extracting name and address from QBluetoothServiceInfo
        device["name"] = d.name();
        device["address"] = d.address().toString();
        devices.append(device);
    }

    return devices;
}
