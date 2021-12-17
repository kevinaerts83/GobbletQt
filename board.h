#ifndef BOARD_H
#define BOARD_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"
#include "board3d.h"

class Board : public QQuickPaintedItem, public Shape
{
    Q_OBJECT
    Q_PROPERTY(double xangle READ xangle WRITE setXangle NOTIFY xangleChanged)
    Q_PROPERTY(double yangle READ xangle WRITE setYangle NOTIFY yangleChanged)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
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

    void paint(QPainter *painter);
private:
    double m_xangle;
    double m_yangle;
    double m_zoom;
signals:
    void xangleChanged(double xangle);
    void yangleChanged(double yangle);
    void zoomChanged(double zoom);
};

#endif // BOARD_H
