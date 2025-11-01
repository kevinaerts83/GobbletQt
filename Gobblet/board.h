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

    Q_PROPERTY(Matrix* matrix MEMBER m_matrix)

    QML_ELEMENT

public:
    Board(QQuickItem *parent = 0);
    Board3d model;
    Matrix* m_matrix = nullptr;

    void paint(QPainter *painter);
};

#endif // BOARD_H
