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

    bool isSelected() const;
    void toggleSelection();

    bool isOnBoard() const;
    void setOnBoard();
private:
    void Zoom(Matrix* matrix);
    bool m_selected = false;
    bool m_onBoard = false; // for animation, Don't switch rotation during movement from stack to board
};

#endif // SHAPE3D_H
