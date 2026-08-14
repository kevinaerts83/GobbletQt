#include "shape3d.h"
#include "matrix.h"
#include "math.h"

void Shape3d::Rotate(Matrix* matrix, double x, double y, double z) {

    this->cache.resize(points.size());

    double translation [4][4];
    matrix->getTranslationMatrix(x, y, z, translation);

    for (int i = 0; i < this->points.size(); i++) {
        this->cache[i] = matrix->MultiplyPointAndMatrix(this->points[i], translation);
    }

    // fill cache
    this->Zoom(matrix);

    double rotationMatrix [4][4];
    if (isOnBoard()) {
        matrix->getRotationMatrix(rotationMatrix);
    } else {
        matrix->getRotateXMatrix(rotationMatrix);
    }

    // rotate the cached values
    for (int i = 0; i < this->cache.size(); i++) {
        this->cache[i] = matrix->MultiplyPointAndMatrix(this->cache[i], rotationMatrix);
    }
};

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

bool Shape3d::isOnBoard() const {
    return m_onBoard;
}

void Shape3d::setOnBoard() {
    m_onBoard = true;
}

// --- Shape3dInstance ---

void Shape3dInstance::Rotate(Matrix* matrix, double x, double y, double z) {
    const auto& points = m_model->points;
    m_cache.resize(points.size());

    double translation [4][4];
    matrix->getTranslationMatrix(x, y, z, translation);

    for (int i = 0; i < points.size(); i++) {
        m_cache[i] = matrix->MultiplyPointAndMatrix(points[i], translation);
    }

    Zoom(matrix);

    double rotationMatrix [4][4];
    if (isOnBoard()) {
        matrix->getRotationMatrix(rotationMatrix);
    } else {
        matrix->getRotateXMatrix(rotationMatrix);
    }

    for (int i = 0; i < m_cache.size(); i++) {
        m_cache[i] = matrix->MultiplyPointAndMatrix(m_cache[i], rotationMatrix);
    }
}

void Shape3dInstance::Zoom(Matrix* matrix) {
    double scalingMatrix [4][4];
    matrix->getScalingMatrix(scalingMatrix);
    for (int i = 0; i < m_cache.size(); i++) {
        m_cache[i] = matrix->MultiplyPointAndMatrix(m_cache[i], scalingMatrix);
    }
}
