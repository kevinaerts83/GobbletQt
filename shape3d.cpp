#include "shape3d.h"
#include "matrix.h"
#include "math.h"

void Shape3d::Rotate(Matrix* matrix, double x, double y, double z) {

    this->cache.clear();

    QVector<double> center = {x, y, z, 1};

    double translation [4][4];
    matrix->getTranslationMatrix(center[0], center[1], center[2], translation);

    for (int i = 0; i < this->points.size(); i++) {
        this->cache.append(matrix->MultiplyPointAndMatrix(this->points[i], translation));
    }

    /*
        // TRANSLATE TO 0,0
        double toZero [4][4];
        matrix->getTranslationMatrix(-512, -384, 0, toZero);
        QVector<double> centerAtZero = matrix->MultiplyPointAndMatrix(center, toZero);

        // ROTATE THE CENTER
        double rotationMatrix [4][4];
        matrix->getRotationMatrix(rotationMatrix);
        // rotate the cached values
        QVector<double> centerRotated =  matrix->MultiplyPointAndMatrix(centerAtZero, rotationMatrix);

        // TRANSLATE TO CENTER
        double moveBack [4][4];
        matrix->getTranslationMatrix(512, 384, 0, moveBack);
        QVector<double> newCenter = matrix->MultiplyPointAndMatrix(centerRotated, moveBack);
    */


    // fill cache
    this->Zoom(matrix);

    double rotationMatrix [4][4];
    matrix->getRotationMatrix(rotationMatrix);

    // rotate the cached values
    for (int i = 0; i < this->cache.size(); i++) {
        this->cache[i] = matrix->MultiplyPointAndMatrix(this->cache[i], rotationMatrix);
    }
};

int Shape3d::PowerOfTwo(int x) {
    /*0:1, 1:2, 2:4, 3:8,
    5:16, 6:32, 7:64, 8:128,
    10:256, 11:512, 12:1024, 13:2048,
    15:4096, 16:8192, 17:16384, 18:32768*/
    return pow(2, x - x / 5);
}

void Shape3d::Zoom(Matrix* matrix) {
    double scalingMatrix [4][4];
    matrix->getScalingMatrix(scalingMatrix);
    // scale the cached values
    for (int i = 0; i < this->cache.size(); i++) {
        this->cache[i] = matrix->MultiplyPointAndMatrix(this->cache[i], scalingMatrix);
    }
};
