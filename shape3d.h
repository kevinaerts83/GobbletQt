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

    Shape3d() {};

    void Rotate(Matrix* matrix);
    int PowerOfTwo(int x);
private:
    void Zoom(Matrix* matrix);
};

#endif // SHAPE3D_H
