#ifndef BOARD_H
#define BOARD_H

#include "shape_item.h"
#include "board3d.h"

class Board : public ShapeItem
{
    Q_OBJECT

    Q_PROPERTY(Matrix* matrix MEMBER m_matrix)

    QML_ELEMENT

public:
    Board(QQuickItem *parent = nullptr);
    Board3d model;
    Matrix* m_matrix = nullptr;

protected:
    void computeGeometry(QVector<TriangleVertex> &vertices) override;
};

#endif // BOARD_H
