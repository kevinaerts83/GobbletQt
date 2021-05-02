#ifndef SHAPE3D_H
#define SHAPE3D_H
#include "matrix.h"

class Shape3d
{
public:
    double points [8][4];
    int faces [10][4];
    double cache [8][4];
    Matrix matrix;

    Shape3d(double width, double height);
    void Rotate(double rotationMatrix[4][4]);
    void Zoom(double scalingMatrix[4][4]);
    int PowerOfTwo(int x);
};

#endif // SHAPE3D_H
