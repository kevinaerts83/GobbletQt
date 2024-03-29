#ifndef GOBBLER_H
#define GOBBLER_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"
#include "gobbler3d.h"

class Gobbler : public QQuickPaintedItem, public Shape
{
    // Q_PROPERTY only work for Q_OBJECT classes, you can only inherit from ONE Q_Object class
    // (QQuickPaintedItem already does) so the properties can't be moved to Shape
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(Matrix* matrix MEMBER m_matrix)

    Q_PROPERTY(double x3d READ x3d WRITE setX3d)
    Q_PROPERTY(double y3d READ y3d WRITE setY3d)
    Q_PROPERTY(double z3d READ z3d WRITE setZ3d)

    QML_ELEMENT

public:
    Gobbler(QQuickItem *parent = 0);
    gobbler3d model;
    Matrix* m_matrix;

    QString name() const;
    void setName(const QString &name);

    QColor color() const;
    void setColor(const QColor &color);

    double x3d() const;
    void setX3d(const double &x3d);
    double y3d() const;
    void setY3d(const double &y3d);
    double z3d() const;
    void setZ3d(const double &z3d);

    void paint(QPainter *painter);

private:
    QString m_name;
    QColor m_color;

    double m_x3d;
    double m_y3d;
    double m_z3d;
};

#endif // GOBBLER_H
