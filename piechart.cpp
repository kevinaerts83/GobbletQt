#include "piechart.h"
#include <QPainter>

//![0]
PieChart::PieChart(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}
//![0]

QString PieChart::name() const
{
    return m_name;
}

void PieChart::setName(const QString &name)
{
    m_name = name;
}

QColor PieChart::color() const
{
    return m_color;
}

void PieChart::setColor(const QColor &color)
{
    m_color = color;
}

//![1]
void PieChart::paint(QPainter *painter)
{
    QPen pen(m_color, 2);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), 90 * 16, 290 * 16);

    /*
     paintPolygon(painter, 50.0, 50.0, 110.0, 50.0, 50.0, 150.0);
     paintPolygon(painter, 110.0, 50.0, 50.0, 150.0, 110.0, 150.0);
     paintPolygon(painter, 50.0, 50.0, 30.0, 30.0, 50.0, 150.0);
     paintPolygon(painter, 30.0, 30.0, 30.0, 130.0, 50.0, 150.0);
     paintPolygon(painter, 50.0, 50.0, 30.0, 30.0, 110.0, 50.0);
     paintPolygon(painter, 30.0, 30.0, 80.0, 30.0, 110.0, 50.0);
     */
}
//![1]
