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
    ~BluetoothManager();

    // Server (peripheral)
    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();

    Q_INVOKABLE QVariantList getDevices();  // Exposed to QML
    Q_INVOKABLE void connectWithAddress(const QString &address);
    Q_INVOKABLE void startDiscovery();
    Q_INVOKABLE void stopDiscovery();

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

signals:
    void sendMessage(const QString &message);
    void serverError(const QString& message);

    // to be processed in the mediator
    void clientMessage(const QString &from, const QString &msg);
    void serverMessage(const QString &from, const QString &msg);

    void clientNameChanged();
    void serverNameChanged();

public slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);
    void onServerError(const QString &message);

private slots:
    void discoveryFinished();

private:
    void connectToDevice(const QBluetoothDeviceInfo &device);

    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    ChatServer *server = nullptr;
    ChatClient *client = nullptr;

    QVector<QBluetoothDeviceInfo> foundDevices;
    QString m_serverName;
    QString m_clientName;

    // Replace with your device’s actual UUIDs
    const QBluetoothUuid serviceUuid =
        QBluetoothUuid(QStringLiteral("{12345678-6398-4c4c-80cb-2cc15d4734d7}"));
    const QBluetoothUuid rxCharUuid =
        QBluetoothUuid(QStringLiteral("{beefbbbb-6398-4c4c-80cb-2cc15d4734d7}"));
    const QBluetoothUuid txCharUuid =
        QBluetoothUuid(QStringLiteral("{feedaaaa-6398-4c4c-80cb-2cc15d4734d7}"));
};

#endif // BLUETOOTHMANAGER_H
