#ifndef SHAPE_H
#define SHAPE_H
#include <QColor>
#include <QPainter>
#include <QVector>
#include "shape3d.h"

class Shape
{
public:
    Shape();

    virtual double getX() = 0;
    virtual double getY() = 0;
    virtual double getZ() = 0;

    void paint(Shape3d model, QPainter *painter);
private:
    bool dotProduct(QVector<QVector<double>> points, QVector<int> face);
    void paintPolygon(QPainter *painter, qreal x1, qreal x2, qreal x3, qreal y1, qreal y2, qreal y3);
};

#endif // SHAPE_H
