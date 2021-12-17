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
    Q_PROPERTY(double xangle READ xangle WRITE setXangle NOTIFY xangleChanged)
    Q_PROPERTY(double yangle READ xangle WRITE setYangle NOTIFY yangleChanged)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

    Q_PROPERTY(double tx READ tx WRITE setTx)
    Q_PROPERTY(double ty READ ty WRITE setTy)
    Q_PROPERTY(double tz READ tz WRITE setTz)
    QML_ELEMENT

public:
    Board(QQuickItem *parent = 0);
    board3d model;

    double xangle() const;
    void setXangle(const double &xangle);

    double yangle() const;
    void setYangle(const double &yangle);

    double zoom() const;
    void setZoom(const double &zoom);

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
    double m_xangle;
    double m_yangle;
    double m_zoom;
    double m_tx;
    double m_ty;
    double m_tz;
signals:
    void xangleChanged(double xangle);
    void yangleChanged(double yangle);
    void zoomChanged(double zoom);
};

#endif // BOARD_H
