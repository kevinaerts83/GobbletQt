#include "bluetoothManager.h"
#include <QDebug>
#include <QCoreApplication>
#include <QBluetoothPermission>

/*
┌────────────────────────────────────────────────────────────┐
│ Primary Service                                            │
│ UUID: 12345678-6398-4c4c-80cb-2cc15d4734d7                 │
└────────────────────────────────────────────────────────────┘
        │
        ├── Characteristic: RX  (Client → Server)
        │
        │   UUID: beefbbbb-6398-4c4c-80cb-2cc15d4734d7
        │
        │   Properties:
        │     • Write
        │     • Write Without Response
        │
        │   Permissions:
        │     • Writeable
        │
        │   Descriptors:
        │     • (none)
        │
        │   Data Flow:
        │     Client  ── write() ──▶  Server
        │
        │   Qt signal:
        │     QLowEnergyService::characteristicWritten()
        │
        └── Characteristic: TX  (Server → Client)
            │
            │   UUID: feedaaaa-6398-4c4c-80cb-2cc15d4734d7
            │
            │   Properties:
            │     • Notify
            │
            │   Permissions:
            │     • Readable (implicit)
            │
            │   Descriptors:
            │     • Client Characteristic Configuration (CCC)
            │       UUID: 00002902-0000-1000-8000-00805f9b34fb
            │
            │   Data Flow:
            │     Server ── notify() ──▶ Client
            │
            │   Qt signal:
            │     QLowEnergyService::characteristicChanged()

createCentral()
   ↓
connectToDevice()
   ↓
ConnectedState
   ↓
discoverServices()
   ↓
serviceDiscovered(uuid)
   ↓
createServiceObject()
   ↓
discoverDetails()
   ↓
ServiceDiscovered
   ↓
RX/TX characteristics valid

You → ask Qt to connect
Qt → connects
Qt → asks device for services
Device → responds
Qt → emits serviceDiscovered(uuid)
You → react
 */

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
    connect(server, &ChatServer::clientConnected, this, [this](const QString &){
        qDebug() << "Client connected";
        setConnected(true);
    });
    connect(server, &ChatServer::clientDisconnected, this, [this](const QString &){
        qDebug() << "Client disconnected";
        setConnected(false);
    });
    connect(server, &ChatServer::serverError, this, &BluetoothManager::onServerError);
    //connect(this, &BluetoothManager::sendToClient, server, &ChatServer::sendMessage);

    server->startServer(serviceUuid, rxCharUuid, txCharUuid, reverseServiceUuid, reverseRxCharUuid, reverseTxCharUuid);
    m_role = Role::Server;

    setServerName("Bluetooth");
}

void BluetoothManager::stopServer()
{
    if (server) {
        server->stopServer();
        server->deleteLater();
        server = nullptr;
    }
    m_role = Role::None;
    setConnected(false);
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

    // Accept a device if EITHER it exposes our service UUID in its advertisement
    // OR it carries a non-empty local name. Matching on the service UUID makes
    // discovery robust on Android, where the peripheral now advertises the
    // 128-bit service UUID in the PRIMARY packet while the local name arrives
    // separately in the SCAN RESPONSE (see ChatServer::startServer). Relying on
    // the name alone could miss the device if the primary packet (no name) is
    // processed before the scan response.
    const QString name = info.name();
    const bool advertisesService = info.serviceUuids().contains(serviceUuid);

    if (name.isEmpty() && !advertisesService)
        return;

    // Deduplicate: by address when available, otherwise by name.
    for (const QBluetoothDeviceInfo &d : std::as_const(foundDevices)) {
        const bool sameAddress = !info.address().isNull()
                                 && d.address() == info.address();
        const bool sameName = !name.isEmpty() && d.name() == name;
        if (sameAddress || sameName) {
            return; // already known
        }
    }

    qDebug() << "Found BLE device:" << info.name() << info.address().toString()
             << "advertisesService:" << advertisesService;
    foundDevices.append(info);
}

void BluetoothManager::discoveryFinished()
{
    qDebug() << "BLE device discovery finished.";
    if (foundDevices.isEmpty())
        qDebug() << "No BLE devices found.";
}

void BluetoothManager::connectWithName(const QString &name)
{
    for (const QBluetoothDeviceInfo &d : std::as_const(foundDevices)) {
        if (d.name().contains(name, Qt::CaseInsensitive)) {
            stopDiscovery();
            connectToDevice(d);
            break;
        }
    }
}

void BluetoothManager::stopClient() {
    if (client) {
        client->disconnect(this); // disconnect all signals
        client->deleteLater();
        client = nullptr;
    }
    m_role = Role::None;
    setConnected(false);
}

void BluetoothManager::connectToDevice(const QBluetoothDeviceInfo &device)
{
#if QT_CONFIG(permissions)
    QBluetoothPermission btPerm;

    auto status = qApp->checkPermission(btPerm);

    if (status == Qt::PermissionStatus::Undetermined) {

        qApp->requestPermission(btPerm, this, [this](const QPermission &p) {
            if (p.status() != Qt::PermissionStatus::Granted) {
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

    qDebug() << "Connecting to device:" << device.name();

    stopClient();
    client = new ChatClient(this);

    connect(client, &ChatClient::connected, this, [this](){
        qDebug() << "Client connected to remote";
        setConnected(true);
    });
    connect(client, &ChatClient::disconnected, this, [this](){
        qDebug() << "Client disconnected";
        setConnected(false);
    });
    connect(client, &ChatClient::messageReceived, this, &BluetoothManager::clientMessage);
    //connect(this, &BluetoothManager::sendToServer, client, &ChatClient::sendMessage);

    // Start BLE connection
    client->startClient(device, serviceUuid, rxCharUuid, txCharUuid, reverseServiceUuid, reverseRxCharUuid, reverseTxCharUuid);
    m_role = Role::Client;

    setClientName(device.name());
}

QVariantList BluetoothManager::getDevices() {
    QVariantList devices;
    for (const QBluetoothDeviceInfo &d : std::as_const(foundDevices)) {
        // Only expose devices that already have a resolved name. A device may be
        // discovered first via its advertised service UUID (empty name) and gain
        // its name once the scan response is processed; it will appear on the
        // next refresh. The QML list connects by name (connectWithName), so
        // hiding nameless entries keeps every listed row connectable.
        if (d.name().isEmpty())
            continue;

        QVariantMap device;
        device["name"] = d.name();
        device["address"] = d.address().toString();
        devices.append(device);
    }
    // start discovery if empty
    if (foundDevices.isEmpty()) {
        startDiscovery();
    }
    return devices;
}

void BluetoothManager::sendMessage(const QString &msg)
{
    if (m_role == Role::Client && client) {
        client->sendMessage(msg);     // RX write
    }
    else if (m_role == Role::Server && server) {
        server->sendMessage(msg);     // TX notify
    }
}

void BluetoothManager::onServerError(const QString &message)
{
    qWarning() << "[BluetoothManager] Server error:" << message;
}
