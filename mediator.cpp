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

void Mediator::onClick(Matrix *matrix, const double x, const double y) {
    if (m_comm->lock())
        return;

    double coord [4];
    matrix -> get3dPoint(coord, x, y, false);
    if (abs(coord[0]) > 300 || abs(coord[2]) > 300) {
        matrix -> get3dPoint(coord, x, y, true);
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

        updateState(roundX, coord[1], roundZ, oldTile, newTile, matrix);
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
void Mediator::updateState(int x, int y, int z, int oldTile, int newTile, Matrix *matrix) {
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
        double angle = (matrix->yangle() + (matrix->isVertical() ? 90 : 0)) * M_PI / 180;

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

    matrx = matrix;
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
        // tests();
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
        updateState(225 - (newT % 4) * 150, newY, 225 -  150 * (newT / 4), move.from(), newT, matrx);
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

void Mediator::tests() {
    AI computer(m_comm->mode());
    /* TRY TO WIN */
    int state [2][4] = {{32768, 2048, 128, 0}, { 0, 0, 0, 0}};
    aiMove move = computer.move(state);
    std::cout << "19 -> 3 : " << move.from() << " -> " << move.to() << std::endl; // take from stack

    int state2 [2][4] = {{32768, 2048, 128, 0}, { 0, 0, 0, 8}}; // white blocks with size 3
    move = computer.move(state2);
    std::cout << "16 -> 3 : " << move.from() << " -> " << move.to() << std::endl;

    int state3 [2][4] = {{32768, 2048, 128, 0}, { 8, 0, 0, 0}}; // white blocks with size 0
    move = computer.move(state3);
    std::cout << "19 and not 3 : " << move.from() << " -> " << move.to() << std::endl;

    int state4 [2][4] = {{49184, 51200, 49280, 0}, { 0, 8, 0, 0}}; // white blocks with size 1 all pawns on board
    move = computer.move(state4);
    std::cout << "Win 5 -> 3 : " << move.from() << " -> " << move.to() << std::endl;
    /* 1100 0000 0010 0000  0000 0000 0000 0000         15 14 13 12
     * 1100 1000 0000 0000  0000 0000 0000 1000         11 10 09 08
     * 1100 0000 1000 0000  0000 0000 0000 0000         07 06 05 04
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00 */

    // don't use a tile that let white win.
    int state5 [2][4] = {{49184, 51200, 49280, 0}, { 8706, 32, 8, 0}}; // white blocks with size 2 all pawns on board
    move = computer.move(state5);
    std::cout << "Win 11 -> 3 : " << move.from() << " -> " << move.to() << std::endl;
    /* 1100 0000 0010 0000  0010 0010 0000 0010         15 14 13 12
     * 1100 1000 0000 0000  0000 0000 0010 0000         11 10 09 08
     * 1100 0000 1000 0000  0000 0000 0000 1000         07 06 05 04
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00 */

    /* DONT LOSE */
    int state6 [2][4] = {{1536, 0, 0, 0}, { 28672, 2048, 128, 8}}; // take from stack
    move = computer.move(state6);
    std::cout << "16 -> 15 : " << move.from() << " -> " << move.to() << std::endl;

    int state7 [2][4] = {{1600, 0, 0, 0}, { 28672, 2048, 128, 8}}; // take from board
    move = computer.move(state7);
    std::cout << "6/9 -> 15 : " << move.from() << " -> " << move.to() << std::endl;

    /* Block if 1 sized is placed on 9 no possibility to block */
    /* 0000 0000 0000 0000  0000 1001 0000 0000         15 14 13 12    0 0 1 0
     * 0000 0000 0000 0000  0010 0000 0000 0010         11 10 09 08    1 0 0 1
     * 0000 0000 0000 0000  0000 0000 0000 1000         07 06 05 04    0 0 0 0
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00    1 0 1 0 */
    int state8 [2][4] = {{0, 0, 0, 0}, { 2304, 8194, 0, 0}}; // take from stack
    move = computer.move(state8);
    std::cout << "16 -> 9 : " << move.from() << " -> " << move.to() << std::endl;

    /* Attack */
    // put on the middle
    int state9 [2][4] = {{1, 2, 0, 0}, { 12, 32, 0, 0}};
    move = computer.move(state9);
    std::cout << "Attack 18 -> 6/9/12/15 : " << move.from() << " -> " << move.to() << std::endl;

    int state10 [2][4] = {{128, 1, 0, 0}, { 8, 4, 2, 0}};
    move = computer.move(state10);
    std::cout << "Attack 16 -> 1 : " << move.from() << " -> " << move.to() << std::endl;

    /* 1 0 0 2
     * 0 2 1 1
     * 0 1 1 2
     * 2 1 0 2 */
    int state11 [2][4] = {{5136, 1, 8, 4}, { 32864, 772, 2, 1024}};
    move = computer.move(state11);
    std::cout << "Attack(11) 6 -> 8 : " << move.from() << " -> " << move.to() << std::endl;

    /* 1 0 0 2
     * 2 2 1 0
     * 1 0 2 1
     * 2 0 1 2 */
    int state12 [2][4] = {{7168, 40, 1, 32}, {32898, 2560, 1056, 16}};
    move = computer.move(state12);
    std::cout << "Attack(12) 4 -> 5 : " << move.from() << " -> " << move.to() << std::endl;

    /* 2 2 0 1         15 14 13 12
     * 0 1 2 1         11 10 09 08
     * 2 1 1 2         07 06 05 04
     * 1 0 0 2         03 02 01 00 */
    int state13 [2][4] = {{1064, 36928, 17, 128}, {33281, 16528, 32768, 4096}};
    move = computer.move(state13);
    std::cout << "Attack(13) 3 -> 12 : " << move.from() << " -> " << move.to() << std::endl;

    /* 1 1 2 2         15 14 13 12
     * 0 1 1 2         11 10 09 08
     * 2 2 2 1         07 06 05 04
     * 2 0 0 1         03 02 01 00 */
    int state14 [2][4] = {{1552, 53248, 128, 1}, {4168, 768, 544, 16512}};
    move = computer.move(state14);
    std::cout << "Attack(14) 14 -> 5 : " << move.from() << " -> " << move.to() << std::endl;


}
