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

    // Start the BLE connection process
    void startClient(const QBluetoothDeviceInfo &deviceInfo,
                     const QBluetoothUuid &serviceUuid,
                     const QBluetoothUuid &rxCharUuid,
                     const QBluetoothUuid &txCharUuid);

signals:
    void connected(const QString &deviceName);
    void disconnected();
    void messageReceived(const QString &sender, const QString &message);
    void socketErrorOccurred(const QString &error);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onControllerConnected();
    void serviceDiscovered(const QBluetoothUuid &uuid);

    void serviceStateChanged(QLowEnergyService::ServiceState newState);
    void characteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void controllerErrorOccurred(QLowEnergyController::Error error);

private:
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyCharacteristic rxChar; // for reading
    QLowEnergyCharacteristic txChar; // for writing
    QBluetoothUuid serviceUuid;
    QBluetoothUuid rxUuid;
    QBluetoothUuid txUuid;
};

#endif // CHATCLIENT_H
