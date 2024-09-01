#include "mediator.h"
#include <QQmlEngine>

Mediator::Mediator(QQuickItem *parent) : QQuickItem(parent)
{
    qmlRegisterType<Mediator>("MyTypes", 1, 0, "Mediator");
}

/*
 * TODO
 * For the gobblers keep the size
 * gobblers keep the dept (how many gobblers are above me adjust when visibility changes
 *
 * keep the side center points the one from the board should be asked to the board class
 * ...
 * */

QList<Gobbler*> Mediator::getWhiteList() const
{
    return m_whiteList;
}

void Mediator::addWhiteItem(Gobbler *gobbler)
{
    if (gobbler) {
        m_whiteList.append(gobbler);
        emit whiteListChanged();
    }
}

QList<Gobbler*> Mediator::getBlackList() const
{
    return m_blackList;
}

void Mediator::addBlackItem(Gobbler *gobbler)
{
    if (gobbler) {
        m_blackList.append(gobbler);
        emit blackListChanged();
    }
}

bool Mediator::isBlackTurn() const
{
    return m_blackTurn;
}

void Mediator::toggleBlackTurn()
{
    m_blackTurn = !m_blackTurn;
}

Gobbler* Mediator::getSelection() const
{
    return m_selection;
}

void Mediator::setSelection(Gobbler* gobbler)
{
    m_selection = gobbler;
}

Board* Mediator::getBoard() const
{
    return m_board;
}

void Mediator::setBoard(Board* board)
{
    m_board = board;
}

void Mediator::repaint()
{
    for (const auto& item : getWhiteList()) {
        item -> update();
    }
    for (const auto& item : getBlackList()) {
        item -> update();
    }
    getBoard() -> update();
}

void Mediator::onClick(Matrix *matrix, const double x, const double y)
{
    double coord [4];
    matrix -> get3dPoint(coord, x, y);

    if (getSelection() != NULL) {
        getSelection()->setX3d(coord[0]);
        getSelection()->setY3d(coord[1]);
        getSelection()->setZ3d(coord[2]);

        setSelection(NULL);
    }

    for (const auto& item : getWhiteList()) {

        if (item->x3d() > coord[0] - 50 && item->x3d() < coord[0] + 50 &&
            item->y3d() > coord[1] - 50 && item->y3d() < coord[1] + 50) {

            setSelection(item);
        }
        item -> update();
    }

    for (const auto& item : getBlackList()) {

        if (item->x3d() > coord[0] - 50 && item->x3d() < coord[0] + 50 &&
            item->y3d() > coord[1] - 50 && item->y3d() < coord[1] + 50) {

            setSelection(item);
        }
        item -> update();
    }
}

