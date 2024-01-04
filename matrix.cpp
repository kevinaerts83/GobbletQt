#include "matrix.h"
#include "cmath"
#include <iostream>

//constructor
Matrix::Matrix(QQuickItem *parent) : QQuickItem(parent)
{
    double width = 1024;
    double height = 768;

    double centerX = width / 2;
    double centerY = height / 2;
    double centerZ = 0;

    double init [4][4] = {{ 1.0, 0, 0, -centerX}, { 0, 1.0, 0, -centerY}, {0, 0, 1.0, -centerZ}, {0, 0, 0, 1.0}};
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            this->m_translation[i][j] = init[i][j];
        }
    }
    double reverse [4][4] = {{ 1.0, 0, 0, centerX}, {0, 1.0, 0, centerY}, {0, 0, 1.0, centerZ}, {0, 0, 0, 1.0}};
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            this->m_inverseTranslation[i][j] = reverse[i][j];
        }
    }
}

QVector<double> Matrix::MultiplyPointAndMatrix(QVector<double> point, double matrix [4][4])
{
    QVector<double> result(4);
    for(int i = 0; i < 4; i += 1) {
        double tmp = 0;
        for(int j = 0; j < 4; j += 1) {
            tmp += point[j] * matrix[i][j];
        }
        result[i] = tmp;
    }
    return result;
}

QVector<double> Matrix::ProjectPoint(QVector<double> podouble)
{
    QVector<double> p = this->MultiplyPointAndMatrix(podouble, this->m_translation);
    double x = p[0],
    y = p[1],
    z = p[2],
    f = -1.0/900.0,
    s = 1;

    QVector<double> p2 = {x / ((z * f) + s), y / ((z * f) + s), 0, 1};

    return this->MultiplyPointAndMatrix(p2, this->m_inverseTranslation);
}

void Matrix::MultiplyMatrixAndMatrix(double matrix1 [4][4], double matrix2 [4][4], double result [4][4])
{
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = (matrix1[i][0] * matrix2[0][j]) +
                              (matrix1[i][1] * matrix2[1][j]) +
                              (matrix1[i][2] * matrix2[2][j]) +
                              (matrix1[i][3] * matrix2[3][j]);
        }
    }
}

void Matrix::getInverseTranslationMatrix(double result [4][4])
{
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = this->m_inverseTranslation[i][j];
        }
    }
}

void Matrix::getTranslationMatrix(double x, double y, double z, double result [4][4])
{
    double init [4][4] = {{1.0, 0, 0, x}, {0, 1.0, 0, y}, {0, 0, 1.0, z}, {0, 0, 0, 1.0}};
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = init[i][j];
        }
    }
}

void Matrix::getScalingMatrix(double result [4][4])
{
    double init [4][4] = {{m_zoom, 0, 0, 0}, {0, m_zoom, 0, 0}, {0, 0, m_zoom, 0}, {0, 0, 0, 1.0}};
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = init[i][j];
        }
    }
}

void Matrix::getRotationMatrix(double result [4][4])
{
    double angleX = m_xangle * M_PI / 180,
        angleY = m_yangle * M_PI / 180,
        cosx = cos(angleX),
        sinx = -sin(angleX),
        cosy = cos(angleY),
        siny = -sin(angleY);
    double xFacesMatrix [4][4] = {{1.0, 0, 0, 0}, {0, cosx, -sinx, 0}, {0, sinx, cosx, 0}, {0, 0, 0, 1.0}};
    double yFacesMatrix [4][4] = {{cosy, 0, siny, 0}, {0, 1.0, 0, 0}, {-siny, 0, cosy, 0}, {0, 0, 0, 1.0}};

    this->MultiplyMatrixAndMatrix(xFacesMatrix, yFacesMatrix, result);
}

void Matrix::get3dPoint(double result [4], const double x, const double y) {

    double tx = x + this->m_translation[0][3];
    double tz = y + this->m_translation[1][3];
    double ty = tz * sin(90 - this->xangle()) / sin(this->xangle());

    double f = -1.0/900.0;
    double s = 1;

    std::cout << tx << std::endl;
    std::cout << ty << std::endl;

    tx = tx * ((tz * f) + s);
    ty = ty * ((tz * f) + s);

    std::cout << tx << std::endl;
    std::cout << ty << std::endl;

    QVector<double> point = {tx, ty, tz, 1};

    this->setZoom(this->zoom() * -1);
    double scalingMatrix [4][4];
    this->getScalingMatrix(scalingMatrix);
    // scale the cached values
    this->MultiplyPointAndMatrix(point, scalingMatrix);
    this->setZoom(this->zoom() * -1);

    this->setXangle(this->m_xangle * -1);
    this->setYangle(this->m_yangle * -1);

    double rotationMatrix [4][4];
    this->getRotationMatrix(rotationMatrix);
    this->MultiplyPointAndMatrix(point, scalingMatrix);

    this->setXangle(this->m_xangle * -1);
    this->setYangle(this->m_yangle * -1);

    for(int i = 0; i < 4; i += 1) {
        result[i] = point[i];
    }
}

double Matrix::xangle() const { return m_xangle; }
double Matrix::yangle() const { return m_yangle; }
double Matrix::zoom() const { return m_zoom; }

void Matrix::setXangle(const double &xangle)
{
    m_xangle = xangle;
    emit xangleChanged(xangle);
}

void Matrix::setYangle(const double &yangle)
{
    m_yangle = yangle;
    emit yangleChanged(yangle);
}

void Matrix::setZoom(const double &zoom)
{
    m_zoom = zoom;
    emit zoomChanged(zoom);
}
