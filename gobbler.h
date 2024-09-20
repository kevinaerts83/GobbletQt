#ifndef GOBBLER_H
#define GOBBLER_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"
#include "gobbler3d.h"

class Gobbler : public QQuickPaintedItem
{
    // Q_PROPERTY only work for Q_OBJECT classes, you can only inherit from ONE Q_Object class
    // (QQuickPaintedItem already does) so the properties can't be moved to Shape
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(Matrix* matrix MEMBER m_matrix)

    Q_PROPERTY(bool isWhite READ isWhite WRITE setWhite)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(double x3d READ x3d WRITE setX3d NOTIFY x3dChanged)
    Q_PROPERTY(double y3d READ y3d WRITE setY3d NOTIFY y3dChanged)
    Q_PROPERTY(double z3d READ z3d WRITE setZ3d NOTIFY z3dChanged)

    QML_ELEMENT

public:
    Gobbler(QQuickItem *parent = 0, Shape *shape = nullptr, Gobbler3d model = NULL);
    Gobbler3d model;
    Matrix *m_matrix;
    Shape *m_shape;

    QString name() const;
    void setName(const QString &name);

    bool isWhite() const;
    void setWhite(const bool &isWhite);

    int depth() const;
    void setDepth(const int &depth);

    double x3d() const;
    void setX3d(const double &x3d);
    double y3d() const;
    void setY3d(const double &y3d);
    double z3d() const;
    void setZ3d(const double &z3d);

    void paint(QPainter *painter);

    static bool compareByZindex(const Gobbler* a, const Gobbler* b);

signals:
    void depthChanged();
    void x3dChanged();
    void y3dChanged();
    void z3dChanged();

private:
    QString m_name;
    bool m_isWhite;
    int m_depth;
    double m_x3d;
    double m_y3d;
    double m_z3d;

};

#endif // GOBBLER_H
