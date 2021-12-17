#ifndef GOBBLER_H
#define GOBBLER_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"
#include "gobbler3d.h"

class Gobbler : public QQuickPaintedItem, public Shape
{
    // Q_PROPERTY only work for Q_OBJECT classes, you can only inherit from ONE Q_Object class
    // (QQuickPaintedItem already does) so the properties can't be moved to Shape
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(double xangle READ xangle WRITE setXangle NOTIFY xangleChanged)
    Q_PROPERTY(double yangle READ xangle WRITE setYangle NOTIFY yangleChanged)
    Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

    Q_PROPERTY(double tx READ tx WRITE setTx)
    Q_PROPERTY(double ty READ ty WRITE setTy)
    Q_PROPERTY(double tz READ tz WRITE setTz)
    QML_ELEMENT

public:
    Gobbler(QQuickItem *parent = 0);
    gobbler3d model;

    QString name() const;
    void setName(const QString &name);

    QColor color() const;
    void setColor(const QColor &color);

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
    QString m_name;
    QColor m_color;
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

#endif // GOBBLER_H
