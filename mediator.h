#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QtQuick/QQuickItem>
#include <QVector>
#include <QObject>
#include <QQuickItem>
#include "gobbler.h"

class Mediator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<Gobbler*> itemList READ getItemList NOTIFY itemListChanged)
    QML_ELEMENT

public:
    Mediator(QQuickItem *parent = 0);
    QList<Gobbler*> getItemList() const;

public slots:
    void addItem(Gobbler *gobbler);
    void repaint();
    void onClick(Matrix *matrix, const double x, const double y);

signals:
    void itemListChanged();

private:
    QList<Gobbler*> m_itemList;
};

#endif // MEDIATOR_H
