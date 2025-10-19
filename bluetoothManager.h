// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QObject>
#include <QBluetoothHostInfo>

class ChatServer;
class ChatClient;

//! [declaration]
class BluetoothManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY clientNameChanged)
    Q_PROPERTY(QString serverName READ serverName WRITE setServerName NOTIFY serverNameChanged)
public:
    explicit BluetoothManager();
    ~BluetoothManager();

    Q_INVOKABLE void initBluetooth();
    Q_INVOKABLE QVariantList getDevices();  // Exposed to QML
    Q_INVOKABLE void connectToDevice(const QString &uuidAddress);

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
    void clientNameChanged();
    void serverNameChanged();

private slots:
    void connectClicked();
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);

private:
    ChatServer *server = nullptr;
    QList<ChatClient *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString m_serverName;
    QString m_clientName;
};
//! [declaration]
