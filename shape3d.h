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

    void Rotate(Matrix* matrix, double x, double y, double z);
    int PowerOfTwo(int x);

    bool isSelected() const;
    void toggleSelection();
private:
    void Zoom(Matrix* matrix);
    bool m_selected;
};

#endif // SHAPE3D_H
