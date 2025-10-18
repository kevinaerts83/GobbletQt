// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "remoteselector.h"

#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothUuid>

using namespace Qt::StringLiterals;

RemoteSelector::RemoteSelector(const QBluetoothAddress &localAdapter)
{
    //! [createDiscoveryAgent]
    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(localAdapter);

    connect(m_discoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &RemoteSelector::serviceDiscovered);
    connect(m_discoveryAgent, &QBluetoothServiceDiscoveryAgent::finished,
            this, &RemoteSelector::discoveryFinished);
    connect(m_discoveryAgent, &QBluetoothServiceDiscoveryAgent::canceled,
            this, &RemoteSelector::discoveryFinished);
    //! [createDiscoveryAgent]
}

RemoteSelector::~RemoteSelector()
{
    delete m_discoveryAgent;
}

void RemoteSelector::startDiscovery(const QBluetoothUuid &uuid)
{
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    //! [startDiscovery]
    m_discoveryAgent->setUuidFilter(uuid);
    m_discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
    //! [startDiscovery]
}

void RemoteSelector::stopDiscovery()
{
    if (m_discoveryAgent){
        m_discoveryAgent->stop();
    }
}

QBluetoothServiceInfo RemoteSelector::service() const
{
    return m_service;
}

QVariantList RemoteSelector::getDevices() {
    QVariantList devices;

    QVariantMap device1;
    device1["name"] = "Speaker Pro";
    device1["address"] = "00:11:22:33:44:55";
    devices.append(device1);

    QVariantMap device2;
    device2["name"] = "Wireless Headphones";
    device2["address"] = "AA:BB:CC:DD:EE:FF";
    devices.append(device2);

    return devices;
}

void RemoteSelector::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    const QBluetoothAddress address = serviceInfo.device().address();
    for (const QBluetoothServiceInfo &info : std::as_const(m_discoveredServices)) {
        if (info.device().address() == address)
            return;
    }

    //! [serviceDiscovered]
    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = address.toString();
    else
        remoteName = serviceInfo.device().name();

// QListWidgetItem *item =
//        new QListWidgetItem(QString::fromLatin1("%1 %2").arg(remoteName,
//                                                             serviceInfo.serviceName()));

//    m_discoveredServices.insert(item, serviceInfo);
//    ui->remoteDevices->addItem(item);
    //! [serviceDiscovered]
}

void RemoteSelector::discoveryFinished()
{
    //ui->status->setText(tr("Select the chat service to connect to."));
}

void RemoteSelector::remoteDevicesActivated()
{
    //m_service = m_discoveredServices.value(item);
    //if (m_discoveryAgent->isActive())
    //    m_discoveryAgent->stop();

    //accept();
}

// QListWidgetItem *
void RemoteSelector::remoteDevicesClicked()
{
    //ui->connectButton->setEnabled(true);
    //ui->connectButton->setFocus();
}

void RemoteSelector::connectClicked()
{
    //auto items = ui->remoteDevices->selectedItems();
    //if (items.size()) {
    //    QListWidgetItem *item = items[0];
    //    m_service = m_discoveredServices.value(item);
    //    if (m_discoveryAgent->isActive())
    //        m_discoveryAgent->stop();

    //    accept();
    //}
}

void RemoteSelector::cancelClicked()
{
    //reject();
}
