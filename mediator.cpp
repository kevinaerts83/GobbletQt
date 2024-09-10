#include "mediator.h"
#include <QQmlEngine>
#include <iostream>
#include <ostream>

#include "gobbler.h"

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

QList<Gobbler*> Mediator::getList() const
{
    return m_list;
}

void Mediator::addItem(Gobbler *gobbler)
{
    if (gobbler) {
        m_list.append(gobbler);
        emit listChanged();
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
    getBoard()->update();

    std::sort(m_list.begin(), m_list.end(), Gobbler::compareByZindex);
    for (int i = 0; i < m_list.size(); i++) {
        m_list[i]->setZ(i);
    }
    for (const auto& item : m_list) {
        if (item->depth() == 0) {
            item->update();
        }
    }
}

void Mediator::onClick(Matrix *matrix, const double x, const double y)
{
    double coord [4];
    matrix -> get3dPoint(coord, x, y);

    int roundX = ceil(coord[0] / 150) * 150 - 75;
    int roundZ = ceil(coord[2] / 150) * 150 - 75;
    int borderZ = (abs(roundX) > 225) ? ((coord[2] > 75) ? 150 : ((coord[2] < -75) ? -150 : 0)) : roundZ;

    if (getSelection() != NULL) {

        for (const auto& item : getList()) {
            if (item->depth() > getSelection()->depth()
                && item->x3d() == getSelection()->x3d()
                && item->z3d() == getSelection()->z3d()) {

                item->setDepth(item->depth() - 1);
            }

            if (item->x3d() == roundX && item->z3d() == roundZ) {
                item->setDepth(item->depth() + 1);
            }
        }

        getSelection()->setX3d(roundX);
        getSelection()->setY3d(coord[1]);
        getSelection()->setZ3d(roundZ);

        setSelection(NULL);
    } else {
        for (const auto& item : getList()) {
            if (item->depth() == 0 && item->x3d() == roundX && item->z3d() == borderZ) {
                setSelection(item);
            }
        }
    }
    if (m_selection == NULL) {
        repaint();
    }
}

