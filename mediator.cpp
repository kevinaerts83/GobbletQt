#include "mediator.h"
#include <QQmlEngine>
#include <iostream>
#include <bitset>
#include <string>

#include "gobbler.h"
#include "ai.h"

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

void Mediator::setSelection(int roundX, int borderZ)
{
    for (const auto& item : getList()) {
        if (item->depth() == 0 && item->x3d() == roundX && item->z3d() == borderZ) {
            setSelection(item);
        }
    }
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

        // update state of old tile
        if ((((int) getSelection()->z3d()) % 150) != 0) {
            int tile = 15 - ((getSelection()->x3d() + 225) / 150 + ((getSelection()->z3d() + 225) / 150) * 4);
            if(tile > -1) {
                m_state[getSelection()->isWhite()][getSelection()->size()] ^= (int)pow(2, tile); //Clear previous position
            }
        }

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

        // set to new position
        getSelection()->setX3d(roundX);
        getSelection()->setY3d(coord[1]);
        getSelection()->setZ3d(roundZ);

        // update state of new tile
        int newTile = 15 - ((getSelection()->x3d() + 225) / 150 + ((getSelection()->z3d() + 225) / 150) * 4);
        m_state[getSelection()->isWhite()][getSelection()->size()] |= (int) pow(2, newTile); //Set new position

        // first check the player who's turn is next (In case when a gobblet of the opposite player is revealed)
        if (getSelection()->isWhite()) {
            if (checkWinner(0)) {
                std::cout << "Black won " << std::endl;
            } else if (checkWinner(1)) {
                std::cout << "White won " << std::endl;
            }
        } else {
            if (checkWinner(1)) {
                std::cout << "White won " << std::endl;
            } else if (checkWinner(0)) {
                std::cout << "Black won " << std::endl;
            }
        }

        setSelection(NULL);

        // writeLog();

        AI computer;
        aiMove move = computer.move(m_state);
        std::cout << move.from() << std::endl;
        // tile to roundX and borderZ
        // setSelection(roundX, borderZ);
        std::cout << move.to() << std::endl;

    } else {
        setSelection(roundX, borderZ);
    }
    if (m_selection == NULL) {
        repaint();
    }
}

/*
  Check if one of the following values occur.

  1111 0000 0000 0000
  0000 1111 0000 0000
  0000 0000 1111 0000
  0000 0000 0000 1111
  1000 1000 1000 1000
  0100 0100 0100 0100
  0010 0010 0010 0010
  0001 0001 0001 0001
  1000 0100 0010 0001
  0001 0010 0100 1000
*/
bool Mediator::checkWinner(bool player) {
    int winner [4] = {m_state[player][0], m_state[player][1], m_state[player][2], m_state[player][3]};
    int loser [4] = {m_state[player ^ 1][0], m_state[player ^ 1][1], m_state[player ^ 1][2], m_state[player ^ 1][3]};
    int chkPlayer [4];
    chkPlayer[0] = winner[0];
    chkPlayer[1] = (winner[1] ^ loser[0]) & winner[1];
    chkPlayer[2] = (winner[2] ^ (loser[0] | loser[1])) & winner[2];
    chkPlayer[3] = (winner[3] ^ (loser[0] | loser[1] | loser[2])) & winner[3];

    int number = chkPlayer[0] | chkPlayer[1] | chkPlayer[2] | chkPlayer[3];

    bool won = false;
    for (int i = 0; i < 10; i++) {
        if ((m_mask[i] & number) == m_mask[i]) {
            won = true;
            break;
        }
    }
    return won;
}


void Mediator::writeLog() {
    std::bitset<16> one(65536 + m_state[0][0]);
    std::bitset<16> two(65536 + m_state[0][1]);
    std::bitset<16> three(65536 + m_state[0][2]);
    std::bitset<16> four(65536 + m_state[0][3]);

    std::cout << "black" << std::endl;
    std::cout << one.to_string().substr(0, 4) << " " << one.to_string().substr(4, 4) << " " << one.to_string().substr(8, 4) << " " << one.to_string().substr(12, 4) << std::endl;
    std::cout << two.to_string().substr(0, 4) << " " << two.to_string().substr(4, 4) << " " << two.to_string().substr(8, 4) << " " << two.to_string().substr(12, 4) << std::endl;
    std::cout << three.to_string().substr(0, 4) << " " << three.to_string().substr(4, 4) << " " << three.to_string().substr(8, 4) << " " << three.to_string().substr(12, 4) << std::endl;
    std::cout << four.to_string().substr(0, 4) << " " << four.to_string().substr(4, 4) << " " << four.to_string().substr(8, 4) << " " << four.to_string().substr(12, 4) << std::endl;

    one = std::bitset<16>(65536 + m_state[1][0]);
    two = std::bitset<16>(65536 + m_state[1][1]);
    three = std::bitset<16>(65536 + m_state[1][2]);
    four = std::bitset<16>(65536 + m_state[1][3]);

    std::cout << "white" << std::endl;
    std::cout << one.to_string().substr(0, 4) << " " << one.to_string().substr(4, 4) << " " << one.to_string().substr(8, 4) << " " << one.to_string().substr(12, 4) << std::endl;
    std::cout << two.to_string().substr(0, 4) << " " << two.to_string().substr(4, 4) << " " << two.to_string().substr(8, 4) << " " << two.to_string().substr(12, 4) << std::endl;
    std::cout << three.to_string().substr(0, 4) << " " << three.to_string().substr(4, 4) << " " << three.to_string().substr(8, 4) << " " << three.to_string().substr(12, 4) << std::endl;
    std::cout << four.to_string().substr(0, 4) << " " << four.to_string().substr(4, 4) << " " << four.to_string().substr(8, 4) << " " << four.to_string().substr(12, 4) << std::endl;
}
