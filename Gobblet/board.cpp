#include "board.h"

Board::Board(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new Board3d();
}

void Board::paint(QPainter *painter)
{
    model.Rotate(m_matrix, 0, 0, 0);
    Shape::paint(m_matrix, model, painter);
}
