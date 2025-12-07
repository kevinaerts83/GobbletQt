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

    // Start the BLE connection process
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
    void sendMessage(const QString &message);

private slots:
    void controllerStateChanged(QLowEnergyController::ControllerState state);
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceDetailsDiscovered();
    void updateNotification(const QLowEnergyCharacteristic &info, const QByteArray &value);

    void serviceStateChanged(QLowEnergyService::ServiceState newState);

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
