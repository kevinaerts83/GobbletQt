// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QObject>
#include <QBluetoothHostInfo>

class ChatServer;
class ChatClient;

//! [declaration]
class Chat : public QObject
{
    Q_OBJECT

public:
    explicit Chat();
    ~Chat();

signals:
    void sendMessage(const QString &message);

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
};
//! [declaration]
