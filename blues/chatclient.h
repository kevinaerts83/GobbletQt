#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyDescriptor>
#include <QBluetoothDeviceInfo>
#include <QDebug>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    ~ChatClient();

    // Start the BLE connection process
    void startClient(const QBluetoothDeviceInfo &deviceInfo,
                     const QBluetoothUuid &serviceUuid,
                     const QBluetoothUuid &rxCharUuid,
                     const QBluetoothUuid &txCharUuid);

    void disconnectClient();
    void sendMessage(const QString &message);

signals:
    void connected(const QString &deviceName);
    void disconnected();
    void messageReceived(const QString &sender, const QString &message);
    void socketErrorOccurred(const QString &error);

private slots:
    void onControllerConnected();
    void onControllerDisconnected();
    void onServiceDiscovered(const QBluetoothUuid &uuid);

    void serviceStateChanged(QLowEnergyService::ServiceState newState);
    void characteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value);

private:
    void cleanupController();

    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyCharacteristic rxChar; // for reading
    QLowEnergyCharacteristic txChar; // for writing
    QBluetoothUuid serviceUuid;
    QBluetoothUuid rxUuid;
    QBluetoothUuid txUuid;
};

#endif // CHATCLIENT_H
