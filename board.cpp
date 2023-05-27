#include "board.h"

//![0]
Board::Board(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new board3d();
}

//![0]
double Board::tx() const
{
    return m_tx;
}

void Board::setTx(const double &tx)
{
    m_tx = tx;
}

double Board::ty() const
{
    return m_ty;
}

void Board::setTy(const double &ty)
{
    m_ty = ty;
}

double Board::tz() const
{
    return m_tz;
}

void Board::setTz(const double &tz)
{
    m_tz = tz;
}

double Board::getX() {
    return m_tx;
}

double Board::getY() {
    return m_ty;
}

double Board::getZ() {
    return m_tz;
}

//![1]
void Board::paint(QPainter *painter)
{
    model.Rotate(m_matrix, getX(), getY(), getZ());
    Shape::paint(m_matrix, model, painter);
}
//![1]
