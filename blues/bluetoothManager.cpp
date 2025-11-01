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
#if QT_CONFIG(permissions)
    QBluetoothPermission permission{};
    switch (qApp->checkPermission(permission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(permission, this, &BluetoothManager::initBluetooth);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning() << "Bluetooth permission denied";
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
#endif

    qDebug() << "Initializing BLE ChatServer...";

    // No need for QBluetoothLocalDevice::HostDiscoverable — BLE advertising replaces that

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

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothManager::discoveryFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, []() { qDebug() << "Discovery canceled."; });
}

void BluetoothManager::startDiscovery()
{
    if (discoveryAgent == nullptr) {
        initClient();
    }

    qDebug() << "Starting BLE device discovery...";
    foundDevices.clear();
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BluetoothManager::stopDiscovery()
{
    if (discoveryAgent->isActive())
        discoveryAgent->stop();
}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (!(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration))
        return;

    qDebug() << "Found BLE device:" << info.name() << info.address().toString();
    foundDevices.append(info);

    stopDiscovery();
    // Example: Auto-connect to first device whose name matches "MyBLEDevice"
    //if (info.name().contains("Gobblet Online", Qt::CaseInsensitive)) {
    //    stopDiscovery();
    //    connectToDevice(info);
    //}
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



/*


void BluetoothManager::startBleDiscovery(const QBluetoothUuid &serviceUuid)
{
    qDebug() << "Starting BLE device discovery...";

    // Create a discovery agent to find BLE devices
    auto *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, [this, serviceUuid](const QBluetoothDeviceInfo &info) {
                if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
                    qDebug() << "Found BLE device:" << info.name() << info.address().toString();

                    // Optionally, filter by known device name or address
                    // if (info.name().contains("MyDevice")) {

                    discoveryAgent()->stop();

                    // Create a controller for the BLE device
                    auto *controller = QLowEnergyController::createCentral(info, this);

                    connect(controller, &QLowEnergyController::connected, this, [this, controller]() {
                        qDebug() << "Connected to BLE device, discovering services...";
                        controller->discoverServices();
                    });

                    connect(controller, &QLowEnergyController::serviceDiscovered, this, [this, serviceUuid, controller](const QBluetoothUuid &foundUuid) {
                        qDebug() << "Service discovered:" << foundUuid.toString();
                        if (foundUuid == serviceUuid) {
                            qDebug() << "Target BLE service found!";
                            controller->connectToService(foundUuid);
                        }
                    });

                    connect(controller, &QLowEnergyController::discoveryFinished, this, []() {
                        qDebug() << "Service discovery finished.";
                    });

                    connect(controller, &QLowEnergyController::errorOccurred, this, [](QLowEnergyController::Error error) {
                        qWarning() << "Controller error:" << error;
                    });

                    connect(controller, &QLowEnergyController::disconnected, this, []() {
                        qDebug() << "BLE device disconnected.";
                    });

                    controller->connectToDevice();
                    // }
                }
            });

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, []() {
        qDebug() << "BLE discovery finished.";
    });

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, []() {
        qDebug() << "BLE discovery canceled.";
    });

    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}
*/
