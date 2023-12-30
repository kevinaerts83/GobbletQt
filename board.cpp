#include "board.h"

//![0]
Board::Board(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new board3d();
}

//![1]
void Board::paint(QPainter *painter)
{
    model.Rotate(m_matrix, 0, 0, 0);
    Shape::paint(m_matrix, model, painter);
}
//![1]
