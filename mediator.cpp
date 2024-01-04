#include "mediator.h"
#include <QQmlEngine>

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

void Mediator::onClick(Matrix *matrix, const double x, const double y)
{
    double coord [4];
    matrix -> get3dPoint(coord, x, y);

    for (const auto& item : getItemList()) {
        item->setX3d(coord[0]);
        item->setY3d(coord[1]);
        item->setZ3d(coord[2]);
        item -> update();
    }
}

