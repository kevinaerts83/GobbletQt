#include "shape3d.h"
#include "matrix.h"
#include "math.h"

Shape3d::Shape3d(double width, double height)
{
    this->matrix = *new Matrix(width, height);
}

void Shape3d::Rotate(double rotationMatrix[4][4]) {
    int length = sizeof(this->points);

    for (int i = 0; i < length; i++) {
        double rotatedPoints [length];
        this->matrix.MultiplyPointAndMatrix(this->points[i], rotationMatrix, rotatedPoints);
        for (int j = 0; j < 4; j++) {
            this->cache[i][j] = rotatedPoints[j];
        }
    }
};

void Shape3d::Zoom(double scalingMatrix[4][4]) {
    int length = sizeof(this->points);

    for (int i = 0; i < length; i++) {
        double scaledPoints [length];
        this->matrix.MultiplyPointAndMatrix(this->points[i], scalingMatrix, scaledPoints);
        for (int j = 0; j < 4; j++) {
            this->cache[i][j] = scaledPoints[j];
        }
    }
};

int Shape3d::PowerOfTwo(int x) {
    /*0:1, 1:2, 2:4, 3:8,
    5:16, 6:32, 7:64, 8:128,
    10:256, 11:512, 12:1024, 13:2048,
    15:4096, 16:8192, 17:16384, 18:32768*/
    return pow(2, x - x / 5);
}
