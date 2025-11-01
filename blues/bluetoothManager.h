#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QVector>
#include "chatclient.h"
#include "chatserver.h"

class BluetoothManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString serverName READ serverName WRITE setServerName NOTIFY serverNameChanged)
public:
    explicit BluetoothManager(QObject *parent = nullptr);

    Q_INVOKABLE void initBluetooth();
    Q_INVOKABLE QVariantList getDevices();  // Exposed to QML
    Q_INVOKABLE void connectWithAddress(const QString &address);

    QString clientName() const {
        return m_clientName;
    }

    void setClientName(QString value) {
        m_clientName = value;
        emit clientNameChanged();
    }

    QString serverName() const {
        return m_serverName;
    }

    void setServerName(QString value) {
        m_serverName = value;
        emit serverNameChanged();
    }

    void connectToDevice(const QBluetoothDeviceInfo &device);
    void initClient();
    void startDiscovery();
    void stopDiscovery();

signals:
    void connected(const QString &deviceName);
    void clientDisconnected();
    void showMessage(const QString &sender, const QString &message);
    void reactOnSocketError(const QString &error);
    void sendMessage(const QString &message);

    void clientNameChanged();
    void serverNameChanged();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void discoveryFinished();

private:
    ChatServer *server = nullptr;
    ChatClient *client = nullptr;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QVector<QBluetoothDeviceInfo> foundDevices;

    QString m_serverName;
    QString m_clientName;

    // Replace with your device’s actual UUIDs    
    const QBluetoothUuid serviceUuid =
        QBluetoothUuid(QStringLiteral("{be828aca-6398-4c4c-80cb-2cc15d4734d7}"));
    const QBluetoothUuid rxCharUuid =
        QBluetoothUuid(QStringLiteral("{be828aca-6398-4c4c-80cb-2cc15d4734d7}"));
    const QBluetoothUuid txCharUuid =
        QBluetoothUuid(QStringLiteral("{be828aca-6398-4c4c-80cb-2cc15d4734d7}"));
};

#endif // BLUETOOTHMANAGER_H
