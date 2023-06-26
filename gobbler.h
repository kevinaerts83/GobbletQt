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

    Q_PROPERTY(double x2d READ x2d WRITE setX2d)
    Q_PROPERTY(double y2d READ y2d WRITE setY2d)

    Q_PROPERTY(double mousex READ mousex WRITE setMousex)
    Q_PROPERTY(double mousey READ mousey WRITE setMousey)
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

    double x2d() const;
    void setX2d(const double &x2d);
    double y2d() const;
    void setY2d(const double &y2d);

    double mousex() const;
    void setMousex(const double &mousex);
    double mousey() const;
    void setMousey(const double &mousey);

    void paint(QPainter *painter);

private:
    QString m_name;
    QColor m_color;

    double m_x3d;
    double m_y3d;
    double m_z3d;

    double m_x2d;
    double m_y2d;

    double m_mousex;
    double m_mousey;
};

#endif // GOBBLER_H
