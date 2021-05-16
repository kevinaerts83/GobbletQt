#ifndef SHAPE_H
#define SHAPE_H
#include <QColor>
#include <QPainter>

class Shape
{
public:
    Shape();

    void paint(QPainter *painter);

private:
    void paintPolygon(QPainter *painter, qreal x1, qreal x2, qreal x3, qreal y1, qreal y2, qreal y3);
};

#endif // SHAPE_H
