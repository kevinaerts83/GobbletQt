#include "board.h"

//![0]
Board::Board(QQuickItem *parent) : QQuickPaintedItem(parent), Shape()
{
    this->model = *new board3d();
}
//![0]
double Board::xangle() const
{
    return m_xangle;
}

void Board::setXangle(const double &xangle)
{
    m_xangle = xangle;
    emit xangleChanged(xangle);
    model.Rotate(m_xangle, m_yangle, m_zoom);
    update();
}

double Board::yangle() const
{
    return m_yangle;
}

void Board::setYangle(const double &yangle)
{
    m_yangle = yangle;
    emit yangleChanged(yangle);
    model.Rotate(m_xangle, m_yangle, m_zoom);
    update();
}

double Board::zoom() const
{
    return m_zoom;
}

void Board::setZoom(const double &zoom)
{
    m_zoom = zoom;
    emit zoomChanged(zoom);
    model.Rotate(m_xangle, m_yangle, m_zoom);
    update();
}

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
    Shape::paint(model, painter);
}
//![1]
