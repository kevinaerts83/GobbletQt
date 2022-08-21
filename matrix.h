#ifndef MATRIX_H
#define MATRIX_H

#include <QtQuick/QQuickItem>
#include <QObject>
#include <QVector>

class Matrix : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(double xangle READ xangle WRITE setXangle NOTIFY xangleChanged)
    Q_PROPERTY(double yangle READ xangle WRITE setYangle NOTIFY yangleChanged)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

    QML_ELEMENT

public:
    Matrix(QQuickItem *parent = 0);
    //virtual ~Matrix() {}
    //Matrix(double width, double height);

    /*static Matrix & instance(double width, double height) {
        static Matrix * _instance = 0;
        if ( _instance == 0 ) {
            _instance = new Matrix(width, height);
        }
        return *_instance;
    }*/

    QVector<double> MultiplyPointAndMatrix(QVector<double> point, double matrix [4][4]);
    QVector<double> ProjectPoint(QVector<double> point);
    void MultiplyMatrixAndMatrix(double matrix1 [4][4], double matrix2 [4][4], double result [4][4]);

    void getTranslationMatrix(double x, double y, double z, double result [4][4]);
    void getScalingMatrix(double result [4][4]);
    void getRotationMatrix(double result [4][4]);

    double xangle() const;
    void setXangle(const double &xangle);
    double yangle() const;
    void setYangle(const double &yangle);
    double zoom() const;
    void setZoom(const double &zoom);

private:
    double m_centerX;
    double m_centerY;
    double m_centerZ;
    double m_translation [4][4];
    double m_inverseTranslation [4][4];

    double m_xangle = 45;
    double m_yangle = 0;
    double m_zoom = 1;

signals:
    void xangleChanged(double xangle);
    void yangleChanged(double yangle);
    void zoomChanged(double zoom);
};

#endif // MATRIX_H
