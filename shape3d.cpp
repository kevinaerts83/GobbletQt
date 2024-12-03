#include "shape3d.h"
#include "matrix.h"
#include "math.h"

void Shape3d::Rotate(Matrix* matrix, double x, double y, double z) {

    this->cache.clear();

    double translation [4][4];
    matrix->getTranslationMatrix(x, y, z, translation);

    for (int i = 0; i < this->points.size(); i++) {
        this->cache.append(matrix->MultiplyPointAndMatrix(this->points[i], translation));
    }

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
}

bool Shape3d::isSelected() const {
    return m_selected;
}

void Shape3d::toggleSelection() {
    m_selected = !m_selected;
}
