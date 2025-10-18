// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "bluetoothManager.h"
#include "chatclient.h"
#include "chatserver.h"
#include "remoteselector.h"

#include <QDebug>

#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>


#if QT_CONFIG(permissions)
#include <QCoreApplication>
#include <QPermissions>
#endif

using namespace Qt::StringLiterals;

static constexpr auto serviceUuid = "be828aca-6398-4c4c-80cb-2cc15d4734d7"_L1;
#ifdef Q_OS_ANDROID
static constexpr auto reverseUuid = "d734475d-c12c-cb80-4c4c-9863ca8a82be"_L1;
#endif

BluetoothManager::BluetoothManager()
{
    initBluetooth();
}

BluetoothManager::~BluetoothManager()
{
    qDeleteAll(clients);
}

QVariantList BluetoothManager::getDevices() {
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                          QBluetoothAddress() :
                                          localAdapters.at(0).address();

    RemoteSelector remoteSelector(adapter);
    return remoteSelector.getDevices();
}

void BluetoothManager::connectToDevice(const QString &uuidAddress) {
    setClientName(uuidAddress);
}

QString BluetoothManager::getLocalName() {
    return localName;
}

void BluetoothManager::initBluetooth()
{
#if QT_CONFIG(permissions)
    QBluetoothPermission permission{};
    switch (qApp->checkPermission(permission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(permission, this, &BluetoothManager::initBluetooth);
        return;
    case Qt::PermissionStatus::Denied:
        return;
    case Qt::PermissionStatus::Granted:
        break; // proceed to initialization
    }
#endif // QT_CONFIG(permissions)

    localAdapters = QBluetoothLocalDevice::allDevices();
    // make discoverable
    if (!localAdapters.isEmpty()) {
        QBluetoothLocalDevice adapter(localAdapters.at(0).address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }

    //! [Create Chat Server]
    server = new ChatServer(this);
    connect(server, QOverload<const QString &>::of(&ChatServer::clientConnected),
            this, &BluetoothManager::clientConnected);
    connect(server, QOverload<const QString &>::of(&ChatServer::clientDisconnected),
            this,  QOverload<const QString &>::of(&BluetoothManager::clientDisconnected));
    server->startServer();
    //! [Create Chat Server]

    //! [Get local device name]
    localName = QBluetoothLocalDevice().name();
    //! [Get local device name]
}

//! [clientConnected clientDisconnected]
void BluetoothManager::clientConnected(const QString &name)
{
    //ui->chat->insertPlainText(QString::fromLatin1("%1 has joined chat.\n").arg(name));
}

void BluetoothManager::clientDisconnected(const QString &name)
{
    //ui->chat->insertPlainText(QString::fromLatin1("%1 has left.\n").arg(name));
}
//! [clientConnected clientDisconnected]

//! [connected]
void BluetoothManager::connected(const QString &name)
{
    //ui->chat->insertPlainText(QString::fromLatin1("Joined chat with %1.\n").arg(name));
}
//! [connected]

void BluetoothManager::reactOnSocketError(const QString &error)
{
    //ui->chat->insertPlainText(QString::fromLatin1("%1\n").arg(error));
}

//! [clientDisconnected]
void BluetoothManager::clientDisconnected()
{
    ChatClient *client = qobject_cast<ChatClient *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}
//! [clientDisconnected]

//! [Connect to remote service]
void BluetoothManager::connectClicked()
{
    //ui->connectButton->setEnabled(false);

    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                          QBluetoothAddress() :
                                          localAdapters.at(0).address();

    RemoteSelector remoteSelector(adapter);
#ifdef Q_OS_ANDROID
    // QTBUG-61392
    Q_UNUSED(serviceUuid);
    remoteSelector.startDiscovery(QBluetoothUuid(reverseUuid));
#else
    remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
#endif
    //if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();

        qDebug() << "Connecting to service" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        ChatClient *client = new ChatClient(this);

        //connect(client, &ChatClient::messageReceived,
        //        this, &Chat::showMessage);
        connect(client, &ChatClient::disconnected,
                this, QOverload<>::of(&BluetoothManager::clientDisconnected));
        connect(client, QOverload<const QString &>::of(&ChatClient::connected),
                this, &BluetoothManager::connected);
        connect(client, &ChatClient::socketErrorOccurred,
                this, &BluetoothManager::reactOnSocketError);
        connect(this, &BluetoothManager::sendMessage, client, &ChatClient::sendMessage);
        client->startClient(service);

        clients.append(client);
    //}

    //ui->connectButton->setEnabled(true);
}
//! [Connect to remote service]

/*! [sendClicked]
void Chat::sendClicked()
{
    ui->sendButton->setEnabled(false);
    ui->sendText->setEnabled(false);

    showMessage(localName, ui->sendText->text());
    emit sendMessage(ui->sendText->text());

    ui->sendText->clear();

    ui->sendText->setEnabled(true);
    ui->sendButton->setEnabled(true);
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    // avoid keyboard automatically popping up again on mobile devices
    ui->sendButton->setFocus();
#else
    ui->sendText->setFocus();
#endif
}
*/

/*! [showMessage]
void Chat::showMessage(const QString &sender, const QString &message)
{
    ui->chat->moveCursor(QTextCursor::End);
    ui->chat->insertPlainText(QString::fromLatin1("%1: %2\n").arg(sender, message));
    ui->chat->ensureCursorVisible();
}
*/
