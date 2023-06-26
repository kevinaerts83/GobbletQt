#include "board.h"

//![0]
Board::Board(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new board3d();
}

//![0]
double Board::x3d() const
{
    return m_x3d;
}

void Board::setX3d(const double &x3d)
{
    m_x3d = x3d;
}

double Board::y3d() const
{
    return m_y3d;
}

void Board::setY3d(const double &y3d)
{
    m_y3d = y3d;
}

double Board::z3d() const
{
    return m_z3d;
}

void Board::setZ3d(const double &z3d)
{
    m_z3d = z3d;
}

//![1]
void Board::paint(QPainter *painter)
{
    model.Rotate(m_matrix, x3d(), y3d(), z3d());
    Shape::paint(m_matrix, model, painter);
}
//![1]
