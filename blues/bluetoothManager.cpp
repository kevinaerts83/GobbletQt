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
    stopDiscovery();
    stopServer();
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
                emit serverError("Bluetooth permission denied");
            }
        });
        return;
    }

    if (status == Qt::PermissionStatus::Denied) {
        emit serverError("Bluetooth permission denied");
        return;
    }
#endif

    stopServer();

    qDebug() << "Initializing BLE ChatServer...";

    server = new ChatServer(this);

    connect(server, &ChatServer::messageReceived,
            this, &BluetoothManager::serverMessage);
    connect(server, &ChatServer::clientConnected, this, [](const QString &){ qDebug() << "Client connected"; });
    connect(server, &ChatServer::clientDisconnected, this, [](const QString &){ qDebug() << "Client disconnected"; });
    connect(server, &ChatServer::serverError,
            this, &BluetoothManager::onServerError);
    connect(this, &BluetoothManager::sendMessage,
            server, &ChatServer::sendMessage);

    server->startServer(serviceUuid, rxCharUuid, txCharUuid);

    setServerName("Gobblet Online");
}

void BluetoothManager::stopServer()
{
    if (server) {
        server->stopServer();
        server->deleteLater();
        server = nullptr;
    }
}

void BluetoothManager::startDiscovery()
{
    stopDiscovery();
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // Optional: give macOS enough time
    discoveryAgent->setLowEnergyDiscoveryTimeout(8000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::onDeviceDiscovered);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothManager::discoveryFinished);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &BluetoothManager::discoveryFinished); // macOS workaround

    qDebug() << "Starting BLE device discovery...";

    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BluetoothManager::stopDiscovery()
{
    if (discoveryAgent) {
        discoveryAgent->stop();
        discoveryAgent->deleteLater();
        discoveryAgent = nullptr;
    }
}

void BluetoothManager::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (!(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration))
        return;

    qDebug() << "Found BLE device:" << info.name() << info.address().toString();
    foundDevices.append(info);
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

    connect(client, &ChatClient::connected, this, [](){ qDebug() << "Client connected to remote"; });
    connect(client, &ChatClient::disconnected, this, [](){ qDebug() << "Client disconnected"; });
    connect(client, &ChatClient::messageReceived, this, &BluetoothManager::clientMessage);
    connect(this, &BluetoothManager::sendMessage, client, &ChatClient::sendMessage);

    // Start BLE connection
    client->startClient(device, serviceUuid, rxCharUuid, txCharUuid);

    setClientName("Connected");
}

QVariantList BluetoothManager::getDevices() {
    QVariantList devices;
    for (const QBluetoothDeviceInfo &d : std::as_const(foundDevices)) {
        QVariantMap device;
        // Example: extracting name and address from QBluetoothServiceInfo
        device["name"] = d.name();
        device["address"] = d.address().toString();
        devices.append(device);
    }
    // start discovery if empty
    if (foundDevices.isEmpty()) startDiscovery();

    return devices;
}

void BluetoothManager::onServerError(const QString &message)
{
    qWarning() << "[BluetoothManager] Server error:" << message;
}
