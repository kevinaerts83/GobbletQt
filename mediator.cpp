#include "mediator.h"
#include <QQmlEngine>
#include <iostream>
#include <bitset>
#include <string>

#include <QQmlComponent>
#include <QQmlContext>

#include "gobbler.h"
#include "ai.h"

Mediator::Mediator(QObject *parent) : QObject(parent)
{
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

Gobbler* Mediator::getSelection() const {
    return m_selection;
}

void Mediator::setSelectionByTile(int tile) {
    if (tile > 15) {
        int theSize = tile - 16;
        for (const auto& item : getList()) {
            if (item->depth() == 0
                && item->size() == theSize
                && ((!isBlackTurn() && abs(item->x3d()) > 225 && !item->isWhite())
                || (isBlackTurn() && abs(item->x3d()) < -225 && item->isWhite()))) {

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
    m_selection = gobbler;
}

Board* Mediator::getBoard() const {
    return m_board;
}

void Mediator::setBoard(Board* board) {
    m_board = board;
}

void Mediator::repaint(Matrix *matrix) {
    getBoard()->update();

    double angle = matrix->yangle();

    std::sort(m_list.begin(), m_list.end(), [angle](const Gobbler* a, const Gobbler* b) {
        return Gobbler::compareByZindex(a, b, angle);
    });

    for (int i = 0; i < m_list.size(); i++) {
        m_list[i]->setZ(i);
    }
    for (const auto& item : m_list) {
        if (item->depth() == 0) {
            item->setVisible(true);
            item->update();
        } else if (item->isVisible()) {
            item->setVisible(false);
            item->update();
        }
    }
}

void Mediator::onClick(Matrix *matrix, const double x, const double y) {
    double coord [4];
    matrix -> get3dPoint(coord, x, y);

    int roundX = ceil(coord[0] / 150) * 150 - 75;
    int roundZ = ceil(coord[2] / 150) * 150 - 75;

    if (getSelection() != NULL) {

        int oldTile = -1;
        if ((((int) getSelection()->z3d()) % 150) != 0) {
            oldTile = getTileFromCoord(getSelection()->x3d(), getSelection()->z3d());
        }
        int newTile = getTileFromCoord(roundX, roundZ);

        int size = getSelection()->size();
        int mask = pow(2, newTile);
        for (int i = 0; i <= size; i++) {
            if (((m_state[0][i] | m_state[1][i]) & mask) > 0) {
                setSelection(NULL);
                return;
            }
        }

        updateState(roundX, coord[1], roundZ, oldTile, newTile);
        toggleBlackTurn();
        // writeLog();

        if (m_comm->mode() > 0) {
            AI m_computer = *new AI(m_comm->mode());
            aiMove move = m_computer.move(m_state);
            // std::cout << move.from() << std::endl;
            setSelectionByTile(move.from());
            // std::cout << move.to() << std::endl;
            int newT = move.to();
            updateState(225 - (newT % 4) * 150, coord[1], 225 -  150 * (newT / 4), move.from(), newT);
        }
    } else {
        int borderZ = (abs(roundX) > 225) ? ((coord[2] > 75) ? 150 : ((coord[2] < -75) ? -150 : 0)) : roundZ;
        setSelection(roundX, borderZ);
    }
    if (m_selection == NULL) {
        repaint(matrix);
    }
}

void Mediator::updateState(int x, int y, int z, int oldTile, int newTile) {
    if(oldTile > -1) {
        // update state of oldTile
        m_state[getSelection()->isWhite()][getSelection()->size()] ^= (int)pow(2, oldTile);
    }

    // update depth of gobbers
    for (const auto& item : getList()) {
        if (item->depth() > getSelection()->depth()
            && item->x3d() == getSelection()->x3d()
            && item->z3d() == getSelection()->z3d()) {

            item->setDepth(item->depth() - 1);
        }

        if (item->x3d() == x && item->z3d() == z) {
            item->setDepth(item->depth() + 1);
        }
    }

    // set to new position
    getSelection()->setX3d(x);
    getSelection()->setY3d(y);
    getSelection()->setZ3d(z);

    // update state of new tile
    m_state[getSelection()->isWhite()][getSelection()->size()] |= (int) pow(2, newTile); //Set new position

    // first check the player who's turn is next (In case when a gobblet of the opposite player is revealed)
    if (getSelection()->isWhite()) {
        if (checkWinner(0)) {
            this->m_comm->blackWon();
        } else if (checkWinner(1)) {
            this->m_comm->whiteWon();
        }
    } else {
        if (checkWinner(1)) {
            this->m_comm->whiteWon();
        } else if (checkWinner(0)) {
            this->m_comm->blackWon();
        }
    }

    setSelection(NULL);
    // tests();
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
    AI computer = *new AI(0);
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
    std::cout << "5 -> 3 : " << move.from() << " -> " << move.to() << std::endl;

    // don't use a tile that let white win.
    int state5 [2][4] = {{49184, 51200, 49280, 0}, { 8706, 32, 8, 0}}; // white blocks with size 2 all pawns on board
    move = computer.move(state5);
    std::cout << "11 -> 3 : " << move.from() << " -> " << move.to() << std::endl;
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
    std::cout << "6 -> 15 : " << move.from() << " -> " << move.to() << std::endl;

    /* Block */
    /* 0000 0000 0000 0000  0000 1001 0000 0000         15 14 13 12
     * 0000 0000 0000 0000  0010 0000 0000 0010         11 10 09 08
     * 0000 0000 0000 0000  0000 0000 0000 1000         07 06 05 04
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00 */
    int state8 [2][4] = {{0, 0, 0, 0}, { 2304, 8194, 0, 0}}; // take from board
    move = computer.move(state8);
    std::cout << "16 -> 9 : " << move.from() << " -> " << move.to() << std::endl;


    /* Attack */
}
