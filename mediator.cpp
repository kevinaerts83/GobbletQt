#include "mediator.h"
#include <QQmlEngine>
#include <QTimer>
#include <iostream>
#include <bitset>
#include <string>

#include <QQmlComponent>
#include <QQmlContext>

#include "gobbler.h"
#include "ai.h"

/* TODO
* sort on y-axis
* Smaller build
* Constants
* Singleton
* scroll steps
* Use structs
* QDocs
* */

Mediator::Mediator(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Mediator::updateGobbler);
}

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

Gobbler* Mediator::getSelection() const {
    return m_selection;
}

void Mediator::setSelectionByTile(int tile) {
    if (tile > 15) {
        int theSize = tile - 16;
        for (const auto& item : getList()) {
            if (item->depth() == 0
                && item->size() == theSize
                && ((isBlackTurn() && item->x3d() > 225) || (!isBlackTurn() && item->x3d() < -225))) {

                    setSelection(item);
                    break;
            }
        }
    } else {
        setSelection(225 - (tile % 4) * 150, 225 - 150 * (tile / 4));
    }
}

void Mediator::setSelection(int roundX, int borderZ) {
    for (const auto& item : getList()) {
        if (item->depth() == 0 && item->x3d() == roundX && item->z3d() == borderZ
                && ((!isBlackTurn() && item->isWhite())||(isBlackTurn() && !item->isWhite()))) {
            setSelection(item);
            break;
        }
    }
}

void Mediator::setSelection(Gobbler* gobbler) {
    if (gobbler != nullptr) {
        gobbler->model.toggleSelection();
    }
    if (m_selection != nullptr && m_selection->model.isSelected()) {
        m_selection->model.toggleSelection();
    }
    m_selection = gobbler;
}

Board* Mediator::getBoard() const {
    return m_board;
}

void Mediator::setBoard(Board* board) {
    m_board = board;
}

void Mediator::setMatrix(Matrix* matrix) {
    m_matrix = matrix;
}

void Mediator::repaint() {
    getBoard()->update();

    for (int i = 0; i < m_list.size(); i++) {
        m_list[i]->calculateZIndex();
    }

    std::sort(m_list.begin(), m_list.end(), Gobbler::compareByZindex);

    /*for (int i = 0; i < m_list.size(); i++) {
        if (m_list[i]->depth() == 0) {
            std::cout << m_list[i]->name().toStdString() << " " << m_list[i]->getZIndex() << std::endl;
        }
    }*/

    for (int i = 0; i < m_list.size(); i++) {
        m_list[i]->setZ(i);
    }
    for (const auto& item : std::as_const(m_list)) {
        if (item->depth() == 0 || item->isVisible()) {
            if (!item->isVisible()) {
                item->setVisible(true);
            }
            item->update();
        }
    }
}

void Mediator::onClick(const double x, const double y) {
    if (m_comm->lock())
        return;

    double coord [4];
    m_matrix -> get3dPoint(coord, x, y, false);
    if (abs(coord[0]) > 300 || abs(coord[2]) > 300) {
        m_matrix -> get3dPoint(coord, x, y, true);
    }

    int roundX = ceil(coord[0] / 150) * 150 - 75;
    int roundZ = ceil(coord[2] / 150) * 150 - 75;

    if (getSelection() != nullptr) {

        int oldTile = -1;
        if (getSelection()->model.isOnBoard()) {
            oldTile = getTileFromCoord(getSelection()->x3d(), getSelection()->z3d());
        }

        if (abs(roundX) > 225 || abs(roundZ) > 225) {
            setSelection(nullptr);
            repaint();
            return;
        }

        int newTile = getTileFromCoord(roundX, roundZ);

        int size = getSelection()->size();
        int mask = pow(2, newTile);
        for (int i = 0; i <= size; i++) {
            if (((m_state[0][i] | m_state[1][i]) & mask) > 0) {
                setSelection(nullptr);
                repaint();
                return;
            }
        }

        updateState(roundX, coord[1], roundZ, oldTile, newTile);
    } else {
        int borderZ = (abs(roundX) > 225) ? ((coord[2] > 75) ? 150 : ((coord[2] < -75) ? -150 : 0)) : roundZ;
        setSelection(roundX, borderZ);
        repaint();
    }
}

void Mediator::updateDepthOfGobblers(int x, int z) {
    for (const auto &item : std::as_const(m_list)) {
        if (item->depth() > getSelection()->depth() &&
            item->x3d() == getSelection()->x3d() &&
            item->z3d() == getSelection()->z3d()) {

            item->setDepth(item->depth() - 1);
        }

        if (item->x3d() == x && item->z3d() == z) {
            item->setDepth(item->depth() + 1);
        }
    }
}
void Mediator::updateState(int x, int y, int z, int oldTile, int newTile) {
    if (getSelection() == nullptr) {
        std::cout << "An error occurred: " << oldTile << " - " << newTile
                  << std::endl;
        // writeLog();
        return;
    }
    if (oldTile > -1) {
        // update state of oldTile
        m_state[getSelection()->isWhite()][getSelection()->size()] ^= (int)pow(2, oldTile);
    }

    updateDepthOfGobblers(x, z);

    if (oldTile == -1 || oldTile > 15) {
        double a = static_cast<double>(x);
        double b = static_cast<double>(z);
        double angle = (m_matrix->yangle() + (m_matrix->isVertical() ? 90 : 0)) * M_PI / 180;

        newX = a * cos(angle) - b * sin(angle);
        newY = 0;
        newZ = a * sin(angle) + b * cos(angle);
    } else {
        newX = x;
        newY = y;
        newZ = z;
    }

    boardX = x;
    boardY = y;
    boardZ = z;

    myNewTile = newTile;
    timer->start(20);
}

void Mediator::afterAnimation() {
    for (const auto& item : std::as_const(m_list)) {
        if (item->depth() > 0 && item->isVisible()) {
            item->setVisible(false);
            item->update();
        }
    }

    // update state of new tile
    m_state[getSelection()->isWhite()][getSelection()->size()] |= (int) pow(2, myNewTile); //Set new position

    if (!getSelection()->model.isOnBoard()) {
        getSelection()->setX3d(boardX);
        getSelection()->setY3d(boardY);
        getSelection()->setZ3d(boardZ);
    }
    getSelection()->model.toggleSelection();
    getSelection()->model.setOnBoard();
    // first check the player who's turn is next (In case when a gobblet of the opposite player is revealed)
    bool winner = false;
    if (getSelection()->isWhite()) {
        if (checkWinner(0)) {
            this->m_comm->blackWon();
            winner = true;
        } else if (checkWinner(1)) {
            this->m_comm->whiteWon();
            winner = true;
        }
    } else {
        if (checkWinner(1)) {
            this->m_comm->whiteWon();
            winner = true;
        } else if (checkWinner(0)) {
            this->m_comm->blackWon();
            winner = true;
        }
    }
    if (!winner) {
        bool aiTurn = getSelection()->isWhite();
        setSelection(nullptr);
        startAi(aiTurn);

        m_comm->setLock(false);
    }
}

void Mediator::startAi(bool aiTurn) {
    toggleBlackTurn();
    if (m_comm->mode() > 0 && aiTurn) {
        AI m_computer(m_comm->mode());
        aiMove move = m_computer.move(m_state);
        // std::cout << move.from() << std::endl;
        setSelectionByTile(move.from());
        // std::cout << move.to() << std::endl;
        int newT = move.to();
        updateState(225 - (newT % 4) * 150, newY, 225 -  150 * (newT / 4), move.from(), newT);
    }
}

void Mediator::updateGobbler() {
    m_comm->setLock(true);

    int x = getSelection()->x3d();
    int y = getSelection()->y3d();
    int z = getSelection()->z3d();

    if (newX != x) {
        if (newX < x) {
            x -= (x < (newX + m_speed)) ? (x - newX) : m_speed;
        } else {
            x += (x > (newX - m_speed)) ? (newX - x) : m_speed;
        }
    }

    if (newZ != z) {
        if (newZ < z) {
            z -= (z < (newZ + m_speed)) ? (z - newZ) : m_speed;
        } else {
            z += (z > (newZ - m_speed)) ? (newZ - z) : m_speed;
        }
    }


    if (x == newX && z == newZ) {
        y += (y > (newY - m_speed)) ? (newY - y) : m_speed;
    } else {
        if (y > -200) {
            y -= m_speed * 2;
        }
    }

    getSelection()->setX3d(x);
    getSelection()->setY3d(y);
    getSelection()->setZ3d(z);

    repaint();

    if (x == newX && y == newY && z == newZ) {
        timer->stop();
        afterAnimation();
    }
}

int Mediator::getTileFromCoord(int x, int z) {
    return 15 - ((x + 225) / 150 + ((z + 225) / 150) * 4);
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

