#ifndef BOARD_H
#define BOARD_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"
#include "board3d.h"

class Board : public QQuickPaintedItem, public Shape
{
    // Q_PROPERTY only work for Q_OBJECT classes, you can only inherit from ONE Q_Object class
    // (QQuickPaintedItem already does) so the properties can't be moved to Shape
    Q_OBJECT

    Q_PROPERTY(double tx READ tx WRITE setTx)
    Q_PROPERTY(double ty READ ty WRITE setTy)
    Q_PROPERTY(double tz READ tz WRITE setTz)
    Q_PROPERTY(Matrix* matrix MEMBER m_matrix)

    QML_ELEMENT

public:
    Board(QQuickItem *parent = 0);
    board3d model;
    Matrix* m_matrix;

    double tx() const;
    void setTx(const double &tx);
    double ty() const;
    void setTy(const double &ty);
    double tz() const;
    void setTz(const double &tz);

    double getX();
    double getY();
    double getZ();

    void paint(QPainter *painter);
private:
    double m_tx;
    double m_ty;
    double m_tz;
};

#endif // BOARD_H
