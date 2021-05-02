#include "matrix.h"
#include "cmath"

//constructor
Matrix::Matrix()
{
    Matrix(800.0, 600.0);
}

Matrix::Matrix(double width, double height)
{
    double centerX = width/ 2;
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

void Matrix::MultiplyPointAndMatrix(double point [4], double matrix [4][4], double result [4])
{
    for(int i = 0; i < 4; i += 1) {
        double tmp = 0;
        for(int j = 0; j < 4; j += 1) {
            tmp += point[j] * matrix[i][j];
        }
        result[i] = tmp;
    }
}

void Matrix::ProjectPoint(double podouble [4], double result [4])
{
    double p [] = {0, 0, 0, 1};
    this->MultiplyPointAndMatrix(podouble, this->m_translation, p);

    double x = p[0],
    y = p[1],
    z = p[2],
    f = -1/2000,
    s = 1;

    double p2 [] = {x / ((z * f) + s), y / ((z * f) + s), 0, 1};

    this->MultiplyPointAndMatrix(p2, this->m_inverseTranslation, result);
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

void Matrix::getTranslation(double x, double y, double z, double result [4][4])
{
    double init [4][4] = {{1.0, 0, 0, x}, {0, 1.0, 0, y}, {0, 0, 1.0, z}, {0, 0, 0, 1.0}};
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = init[i][j];
        }
    }
}

void Matrix::getScaling(double scale, double result [4][4])
{
    double init [4][4] = {{scale, 0, 0, 0}, {0, scale, 0, 0}, {0, 0, scale, 0}, {0, 0, 0, 1.0}};
    for(int i = 0; i < 4; i += 1) {
        for(int j = 0; j < 4; j += 1) {
            result[i][j] = init[i][j];
        }
    }
}

void Matrix::getRotationMatrix(double x, double y, double result [4][4])
{
    double angleX = M_PI * x / 180,
        angleY = M_PI * y / 180,
        cosx = cos(angleX),
        sinx = -sin(angleX),
        cosy = cos(angleY),
        siny = -sin(angleY);
    double xFacesMatrix [4][4] = {{1.0, 0, 0, 0}, {0, cosx, -sinx, 0}, {0, sinx, cosx, 0}, {0, 0, 0, 1.0}};
    double yFacesMatrix [4][4] = {{cosy, 0, -siny, 0}, {0, 1.0, 0, 0}, {siny, 0, cosy, 0}, {0, 0, 0, 1.0}};

    this->MultiplyMatrixAndMatrix(xFacesMatrix, yFacesMatrix, result);
}
