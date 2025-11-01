#include "matrix.h"
#include <cmath>

//constructor
Matrix::Matrix(QObject *parent) : QObject(parent)
{

}

Q_INVOKABLE void Matrix::setCenter(double width, double height) {

    if (width > height) {
        setVertical(false);
        setZoom(height / 900);
    } else {
        setVertical(true);
        setZoom(width / 900);
    }

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
        fishEye = (p[2] * -1.0/900.0) + 1;

    QVector<double> p2 = {x / fishEye, y / fishEye, 0, 1};

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

void Matrix::getRotateXMatrix(double result [4][4])
{
    double angleX = m_xangle * M_PI / 180,
        angleY = m_vertical ? -90 * M_PI / 180 : 0,
        cosx = cos(angleX),
        sinx = -sin(angleX),
        cosy = cos(angleY),
        siny = -sin(angleY);
    double xFacesMatrix [4][4] = {{1.0, 0, 0, 0}, {0, cosx, -sinx, 0}, {0, sinx, cosx, 0}, {0, 0, 0, 1.0}};
    double yFacesMatrix [4][4] = {{cosy, 0, siny, 0}, {0, 1.0, 0, 0}, {-siny, 0, cosy, 0}, {0, 0, 0, 1.0}};

    this->MultiplyMatrixAndMatrix(xFacesMatrix, yFacesMatrix, result);
}

void Matrix::getTransposedMatrix(double matrix [4][4], double result [4][4]) {
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = matrix[j][i];
        }
    }
}

void Matrix::get3dPoint(double result [4], const double x, const double y, bool stack) {

    // reverse translate
    double rtx = x + this->m_translation[0][3];
    double rty = y + this->m_translation[1][3];
    double rtz = rty * sin(qDegreesToRadians(90 - this->xangle()));

    // undo fish eye projection
    double fishEye = (rtz * -1.0/900.0) + 1;
    rtx = rtx * fishEye;
    rty = rty * fishEye;

    QVector<double> point = {rtx, rty, rtz, 1};

    // reverse zoom
    double zoom = m_zoom;
    m_zoom = 1 / zoom;
    double scalingMatrix [4][4];
    this->getScalingMatrix(scalingMatrix);
    point = this->MultiplyPointAndMatrix(point, scalingMatrix);
    m_zoom = zoom;

    // reverse rotation
    double rotationMatrix [4][4];
    if (stack) {
        this->getRotateXMatrix(rotationMatrix);
    } else {
        this->getRotationMatrix(rotationMatrix);
    }
    double transposedMatrix [4][4];
    getTransposedMatrix(rotationMatrix, transposedMatrix);
    point = this->MultiplyPointAndMatrix(point, transposedMatrix);

    // project point to base level (z = 0)
    // because we revert the rotation are point has in/de-creased on the z-axis.
    point[0] = point[0] + point[1] * sin(qDegreesToRadians(this->yangle()));
    point[2] = point[2] + point[1] * cos(qDegreesToRadians(this->yangle()));

    // finish method
    result[0] = point[0];
    result[1] = 0;
    result[2] = point[2];
    result[3] = 0;
}

double Matrix::xangle() const { return m_xangle; }
double Matrix::yangle() const { return m_yangle; }
double Matrix::zoom() const { return m_zoom; }
bool Matrix::isVertical() const { return m_vertical; }

void Matrix::setXangle(const double &xangle)
{
    m_xangle = xangle;
}

void Matrix::setYangle(const double &yangle)
{
    m_yangle = yangle;
}

void Matrix::setZoom(const double &zoom)
{
    m_zoom = zoom;
}

void Matrix::setVertical(const bool &vertical)
{
    m_vertical = vertical;
}
