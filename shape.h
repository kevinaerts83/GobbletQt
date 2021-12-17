#ifndef SHAPE_H
#define SHAPE_H
#include <QColor>
#include <QPainter>
#include <QVector>

class Shape
{
public:
    Shape();

    void paint(QVector<QVector<double>> points, QVector<QVector<int>> faces, QPainter *painter);
private:
    bool dotProduct(QVector<QVector<double>> points, QVector<int> face);
    void paintPolygon(QPainter *painter, qreal x1, qreal x2, qreal x3, qreal y1, qreal y2, qreal y3);
};

#endif // SHAPE_H
