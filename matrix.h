#ifndef MATRIX_H
#define MATRIX_H
#include <QVector>

class Matrix
{
private:
    double m_centerX;
    double m_centerY;
    double m_centerZ;
    double m_translation [4][4];
    double m_inverseTranslation [4][4];
public:
    Matrix();
    Matrix(double width, double height);
    QVector<double> MultiplyPointAndMatrix(QVector<double> point, double matrix [4][4]);
    QVector<double> ProjectPoint(QVector<double> point);
    void MultiplyMatrixAndMatrix(double matrix1 [4][4], double matrix2 [4][4], double result [4][4]);

    void getTranslationMatrix(double x, double y, double z, double result [4][4]);
    void getScalingMatrix(double scale, double result [4][4]);
    void getRotationMatrix(double x, double y, double result [4][4]);
};

#endif // MATRIX_H
