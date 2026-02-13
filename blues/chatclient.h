#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    ~ChatClient();

    // Start the BLE connection process (central role)
    void startClient(const QBluetoothDeviceInfo &deviceInfo,
                     const QBluetoothUuid &serviceUuid,
                     const QBluetoothUuid &rxCharUuid,
                     const QBluetoothUuid &txCharUuid);

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &sender, const QString &message);
    void socketErrorOccurred(const QString &error);

public slots:
    // Send message to server (write to RX characteristic)
    void sendMessage(const QString &message);

private slots:
    // Controller lifecycle
    void controllerStateChanged(QLowEnergyController::ControllerState state);
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void startClientPeripheral();
    void serviceScanFinished();

    // Service lifecycle
    void serviceStateChanged(QLowEnergyService::ServiceState state);
    void updateNotification(const QLowEnergyCharacteristic &characteristic,
                               const QByteArray &value);

private:
    void cleanupController();
    bool serviceFound = false;
    bool serviceObjectCreated = false;

    QLowEnergyController *central = nullptr;
    QLowEnergyController *peripheral = nullptr;

    QLowEnergyService *centralService = nullptr;
    QLowEnergyService *peripheralService = nullptr;

    // GATT characteristics
    QLowEnergyCharacteristic rxChar; // client -> server (Write)
    QLowEnergyCharacteristic txChar; // server -> client (Notify)

    // UUIDs
    QBluetoothUuid serviceUuid;
    QBluetoothUuid rxUuid;
    QBluetoothUuid txUuid;
};

#endif // CHATCLIENT_H
