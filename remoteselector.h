// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef REMOTESELECTOR_H
#define REMOTESELECTOR_H

#include <QObject>
#include <QBluetoothServiceInfo>

QT_BEGIN_NAMESPACE

class QBluetoothAddress;
class QBluetoothServiceDiscoveryAgent;
class QBluetoothUuid;
class QListWidgetItem;

namespace Ui {
class RemoteSelector;
}
QT_END_NAMESPACE

class RemoteSelector : public QObject
{
    Q_OBJECT

public:
    explicit RemoteSelector(const QBluetoothAddress &localAdapter);

    ~RemoteSelector();

    void startDiscovery(const QBluetoothUuid &uuid);
    void stopDiscovery();
    QBluetoothServiceInfo service() const;


     Q_INVOKABLE QVariantList getDevices();  // Exposed to QML

private:
    Ui::RemoteSelector *ui;

    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QMap<QListWidgetItem *, QBluetoothServiceInfo> m_discoveredServices;

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void remoteDevicesActivated();
    void remoteDevicesClicked();
    void cancelClicked();
    void connectClicked();
};

#endif // REMOTESELECTOR_H
