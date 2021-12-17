#include "shape3d.h"
#include "matrix.h"
#include "math.h"

Shape3d::Shape3d(double width, double height)
{
    this->matrix = *new Matrix(width, height);
}

void Shape3d::Rotate(double xangle, double yangle, double scale) {

    // fill cache
    this->Zoom(scale);

    double rotationMatrix [4][4];
    this->matrix.getRotationMatrix(xangle, yangle, rotationMatrix);

    // rotate the cached values
    for (int i = 0; i < this->cache.size(); i++) {
        this->cache[i] = this->matrix.MultiplyPointAndMatrix(this->cache[i], rotationMatrix);
    }
};

int Shape3d::PowerOfTwo(int x) {
    /*0:1, 1:2, 2:4, 3:8,
    5:16, 6:32, 7:64, 8:128,
    10:256, 11:512, 12:1024, 13:2048,
    15:4096, 16:8192, 17:16384, 18:32768*/
    return pow(2, x - x / 5);
}

void Shape3d::Zoom(double scale) {
    double scalingMatrix [4][4];
    this->matrix.getScalingMatrix(scale, scalingMatrix);
    this->cache.clear();
    for (int i = 0; i < this->points.size(); i++) {
        this->cache.append(this->matrix.MultiplyPointAndMatrix(this->points[i], scalingMatrix));
    }
};
