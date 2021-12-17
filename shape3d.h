#ifndef SHAPE3D_H
#define SHAPE3D_H
#include "matrix.h"
#include <QVector>

class Shape3d
{
public:
    QVector<QVector<double>> points;
    QVector<QVector<int>> faces;
    QVector<QVector<double>> cache;
    Matrix matrix;

    Shape3d(double width, double height);
    void Rotate(double xangle, double yangle, double scale);
    int PowerOfTwo(int x);
private:
    void Zoom(double scale);
};

#endif // SHAPE3D_H
