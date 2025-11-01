#include "gobbler.h"

Gobbler::Gobbler(QQuickItem *parent, Shape *shape, Gobbler3d model) : QQuickPaintedItem(parent), model(model), m_shape(shape) {
    if (!m_shape) {
        m_shape = new Shape();
    }
}

QString Gobbler::name() const
{
    return m_name;
}

void Gobbler::setName(const QString &name)
{
    m_name = name;
}

bool Gobbler::isWhite() const
{
    return m_isWhite;
}

void Gobbler::setWhite(const bool &isWhite)
{
    m_isWhite = isWhite;
}

int Gobbler::size() const
{
    return m_size;
}

void Gobbler::setSize(const int &size)
{
    m_size = size;
}

int Gobbler::depth() const
{
    return m_depth;
}

void Gobbler::setDepth(const int &depth)
{
    m_depth = depth;
}

double Gobbler::x3d() const
{
    return m_x3d;
}

void Gobbler::setX3d(const double &x3d)
{
    m_x3d = x3d;
}

double Gobbler::y3d() const
{
    return m_y3d;
}

void Gobbler::setY3d(const double &y3d)
{
    m_y3d = y3d;
}

double Gobbler::z3d() const
{
    return m_z3d;
}

void Gobbler::setZ3d(const double &z3d)
{
    m_z3d = z3d;
}

void Gobbler::paint(QPainter *painter)
{
    if (this->depth() == 0) {
        model.Rotate(m_matrix, m_x3d, m_y3d, m_z3d);
        QColor color = m_isWhite ? QColor(QColorConstants::Svg::linen).darker(120) : QColor(QColorConstants::Svg::peru.darker(140));
        painter->setBrush(color);
        m_shape->paint(m_matrix, model, painter);
    }
}

void Gobbler::calculateZIndex() {
    if (depth() > 0) {
        m_zIndex = 0;
        return;
    }
    if (y3d() > 100) {
        m_zIndex = 10000;
        return;
    }

    if (!model.isOnBoard() && y3d() == 0) {
        m_zIndex = m_matrix->isVertical() ? x3d() * -3 : z3d();
    } else {
        double sinus = sin(qDegreesToRadians(m_matrix->yangle()));
        double cosinus = cos(qDegreesToRadians(m_matrix->yangle()));

        m_zIndex = x3d() * sinus + z3d() * cosinus;
    }
}

double Gobbler::getZIndex() const
{
    return m_zIndex;
}

// Static comparator function to sort by z-index (ascending)
bool Gobbler::compareByZindex(const Gobbler* a, const Gobbler* b) {
    return a->m_zIndex < b->m_zIndex;
}
