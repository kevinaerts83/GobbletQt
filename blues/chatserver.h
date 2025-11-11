#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QBluetoothUuid>
#include <QDebug>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

    void startServer();
    void stopServer();

signals:
    void clientConnected(const QString &deviceName);
    void clientDisconnected(const QString &deviceName);
    void messageReceived(const QString &sender, const QString &message);

public slots:
    void sendMessage(const QString &message);

private slots:
    void onCharacteristicWritten(const QLowEnergyCharacteristic &ch, const QByteArray &value);
    void onConnectionStateChanged(QLowEnergyController::ControllerState state);

private:
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;

    QLowEnergyCharacteristic rxChar; // receive from client
    QLowEnergyCharacteristic txChar; // send to client

    QBluetoothUuid serviceUuid =
        QBluetoothUuid(QStringLiteral("{12345678-6398-4c4c-80cb-2cc15d4734d7}"));
    QBluetoothUuid rxCharUuid =
        QBluetoothUuid(QStringLiteral("{beefbbbb-6398-4c4c-80cb-2cc15d4734d7}"));
    QBluetoothUuid txCharUuid =
        QBluetoothUuid(QStringLiteral("{feedaaaa-6398-4c4c-80cb-2cc15d4734d7}"));
};

#endif // CHATSERVER_H
