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
public:
    explicit BluetoothManager();
    ~BluetoothManager();

    Q_INVOKABLE QVariantList getDevices();  // Exposed to QML
    Q_INVOKABLE void connectToDevice(const QString &uuidAddress);

    Q_INVOKABLE QString getLocalName();

    QString clientName() const {
        return m_clientName;
    }

    void setClientName(QString value) {
        m_clientName = value;
    }

signals:
    void sendMessage(const QString &message);
    void clientNameChanged(QString);

private slots:
    void connectClicked();
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);

    void initBluetooth();

private:
    ChatServer *server = nullptr;
    QList<ChatClient *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString localName;
    QString m_clientName;
};
//! [declaration]
