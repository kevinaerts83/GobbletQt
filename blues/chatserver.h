#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothUuid>
#include <QDebug>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    ~ChatServer();

    void startServer(const QBluetoothUuid &serviceUuid,
                     const QBluetoothUuid &rxCharUuid,
                     const QBluetoothUuid &txCharUuid);
    void stopServer();

signals:
    void clientConnected(const QString &deviceName);
    void clientDisconnected(const QString &deviceName);
    void messageReceived(const QString &sender, const QString &message);
    void serverError(const QString &message);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onCharacteristicWritten(const QLowEnergyCharacteristic &ch, const QByteArray &value);
    void onConnectionStateChanged(QLowEnergyController::ControllerState state);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QLowEnergyController *centralController = nullptr;
    QLowEnergyService *centralService = nullptr;
    QLowEnergyCharacteristic centralTxChar;

    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;

    QLowEnergyCharacteristic rxChar; // receive from client
    QLowEnergyCharacteristic txChar; // send to client

    QBluetoothUuid rxUuid;
    QBluetoothUuid txUuid;
};

#endif // CHATSERVER_H
