#include "mediator.h"
#include <QQmlEngine>
#include <iostream>

Mediator::Mediator(QQuickItem *parent) : QQuickItem(parent)
{
    qmlRegisterType<Mediator>("MyTypes", 1, 0, "Mediator");
}

QList<Gobbler*> Mediator::getItemList() const
{
    return m_itemList;
}

void Mediator::addItem(Gobbler *gobbler)
{
    if (gobbler) {
        m_itemList.append(gobbler);
        emit itemListChanged();
    }
}

void Mediator::repaint()
{
    for (const auto& item : getItemList()) {
        item -> update();
    }
}
