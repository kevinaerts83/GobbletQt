#ifndef MATRIX_H
#define MATRIX_H

#include <QtQuick/QQuickItem>
#include <QObject>
#include <QVector>

class Matrix : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double xangle READ xangle WRITE setXangle NOTIFY xangleChanged)
    Q_PROPERTY(double yangle READ yangle WRITE setYangle NOTIFY yangleChanged)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(bool vertical READ isVertical WRITE setVertical NOTIFY verticalChanged)

    QML_ELEMENT

public:
    Matrix(QObject *parent = 0);

    QVector<double> MultiplyPointAndMatrix(QVector<double> point, double matrix [4][4]);
    QVector<double> ProjectPoint(QVector<double> point);
    void MultiplyMatrixAndMatrix(double matrix1 [4][4], double matrix2 [4][4], double result [4][4]);

    void getTranslationMatrix(double x, double y, double z, double result [4][4]);
    void getInverseTranslationMatrix(double result [4][4]);
    void getScalingMatrix(double result [4][4]);
    void getRotationMatrix(double result [4][4]);
    void getRotateXMatrix(double result [4][4]);
    void getTransposedMatrix(double matrix [4][4], double result [4][4]);
    void get3dPoint(double result [4], const double x, const double y, const bool stack);

    Q_INVOKABLE void setCenter(double width, double height);

    double xangle() const;
    void setXangle(const double &xangle);
    double yangle() const;
    void setYangle(const double &yangle);
    double zoom() const;
    void setZoom(const double &zoom);
    bool isVertical() const;
    void setVertical(const bool &vertical);

private:
    double m_centerX;
    double m_centerY;
    double m_centerZ;
    double m_translation [4][4];
    double m_inverseTranslation [4][4];

    double m_xangle = 50;
    double m_yangle = 0;
    double m_zoom = 1;
    bool m_vertical = true;

signals:
    void xangleChanged(double xangle);
    void yangleChanged(double yangle);
    void zoomChanged(double zoom);
    void verticalChanged(bool vertical);
};

#endif // MATRIX_H
