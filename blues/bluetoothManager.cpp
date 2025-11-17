#include "bluetoothManager.h"
#include <QDebug>

#if QT_CONFIG(permissions)
#include <QCoreApplication>
#include <QPermissions>
#endif

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent)
{

}

void BluetoothManager::initBluetooth()
{
//    const QVector<QString> permissions = {
//        "android.permission.BLUETOOTH_CONNECT",
//        "android.permission.BLUETOOTH_SCAN",
//        "android.permission.ACCESS_FINE_LOCATION"
//    };
//    for (const QString &permission : permissions) {
//        auto result = QtAndroidPrivate::requestPermission(permission);
//        if (result == QtAndroidPrivate::PermissionResult::Denied) {
//            qWarning() << "Permission denied:" << permission;
//        }
//    }


#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    QBluetoothPermission permission{};
    switch (qApp->checkPermission(permission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(permission, this, &BluetoothManager::initBluetooth);
        return;
    case Qt::PermissionStatus::Denied:
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
#endif

    qDebug() << "Initializing BLE ChatServer...";

    ChatServer *server = new ChatServer(this);
    connect(server, &ChatServer::messageReceived,
            this, &BluetoothManager::showMessage);
    connect(server, &ChatServer::clientConnected,
            this, &BluetoothManager::connected);
    connect(server, &ChatServer::clientDisconnected,
            this, &BluetoothManager::clientDisconnected);
    connect(this, &BluetoothManager::sendMessage,
            server, &ChatServer::sendMessage);

    server->startServer();

    setServerName("Gobblet Online");
}

void BluetoothManager::initClient()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // Optional: give macOS enough time
    discoveryAgent->setLowEnergyDiscoveryTimeout(10000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::deviceDiscovered);

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
    discoveryAgent->stop();
}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo &info)
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

    connect(client, &ChatClient::messageReceived, this, &BluetoothManager::showMessage);
    connect(client, &ChatClient::disconnected, this, &BluetoothManager::clientDisconnected);
    connect(client, &ChatClient::connected, this, &BluetoothManager::connected);
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
