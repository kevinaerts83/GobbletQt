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

//![1]
void Board::paint(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);

    QVector<QVector<double>> points2d;

    double translation [4][4];
    model.matrix.getTranslationMatrix(100, 100, 100, translation);

    for (int i = 0; i < 29; i++) {
        points2d.append(model.matrix.ProjectPoint(model.matrix.MultiplyPointAndMatrix(model.cache[i], translation)));
    }
    Shape::paint(points2d, model.faces, painter);
}
//![1]
