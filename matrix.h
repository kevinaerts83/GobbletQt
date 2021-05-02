#ifndef MATRIX_H
#define MATRIX_H

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
    void MultiplyPointAndMatrix(double point [4], double matrix [4][4], double result [4]);
    void ProjectPoint(double point [4], double result [4]);
    void MultiplyMatrixAndMatrix(double matrix1 [4][4], double matrix2 [4][4], double result [4][4]);

    void getTranslation(double x, double y, double z, double result [4][4]);
    void getScaling(double scale, double result [4][4]);
    void getRotationMatrix(double x, double y, double result [4][4]);
};

#endif // MATRIX_H
