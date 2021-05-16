#ifndef GOBBLER_H
#define GOBBLER_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"

class Gobbler : public QQuickPaintedItem, public Shape
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    QML_ELEMENT

public:
    Gobbler(QQuickItem *parent = 0);

    QString name() const;
    void setName(const QString &name);

    QColor color() const;
    void setColor(const QColor &color);

    void paint(QPainter *painter);
private:
    QString m_name;
    QColor m_color;
};

#endif // GOBBLER_H
