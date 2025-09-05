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

const QList<Gobbler*> Mediator::getList() const
{
    return std::as_const(m_list);
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
    if (tile > MAX_TILE_INDEX) {
        int theSize = tile - MAX_TILE_INDEX - 1;
        for (const auto& item : getList()) {
            if (item->depth() == 0 && item->size() == theSize && isStack(item->x3d())) {
                    setSelection(item);
                    break;
            }
        }
    } else {
        setSelection(getTileX(tile), getTileY(tile));
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

bool Mediator::onClick(const double x, const double y) {
    if (m_comm->lock()) {
        return true;
    }

    double coord [4];
    m_matrix -> get3dPoint(coord, x, y, false); // from board
    if (isNextToBoard(coord[0], coord[2], true)) {
        m_matrix -> get3dPoint(coord, x, y, true); // from stack
    }

    int roundX = roundCoord(coord[0]);
    int roundZ = roundCoord(coord[2]);

    if (getSelection() != nullptr) {

        int oldTile = -1;
        if (getSelection()->model.isOnBoard()) {
            oldTile = getTileFromCoord(getSelection()->x3d(), getSelection()->z3d());
        }

        if (isNextToBoard(roundX, roundZ, false)) {
            setSelection(nullptr);
            repaint();
            return true;
        }

        int newTile = getTileFromCoord(roundX, roundZ);
        if (!isValidMove(oldTile, newTile)) {
            setSelection(nullptr);
            repaint();
            return false;
        }

        updateState(roundX, coord[1], roundZ, oldTile, newTile);
    } else {
        int borderZ = getBorderZ(roundX, coord[2], roundZ);
        setSelection(roundX, borderZ);
        repaint();
    }

    return true;
}

bool Mediator::isValidMove(int oldTile, int newTile) {
    int size = getSelection()->size();
    bool isEmpty = true;
    bool isBigEnough = false;
    int mask = 1 << newTile;


    for (int i = 0; i <= 3; i++) {
        if (((m_state[0][i] | m_state[1][i]) & mask) > 0) {
            isEmpty = false;
            isBigEnough = size < i;
            break;
        }
    }

    // empty square or gobbler from board and big enough
    if (isEmpty || (oldTile != -1 && isBigEnough)) {
        return true;
    }

    if (isBigEnough) {
        AI m_computer(0);
        return m_computer.validateStackMove(m_state, isBlackTurn(), newTile);
    }
    return false;
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
        m_state[getSelection()->isWhite()][getSelection()->size()] ^= (1 << oldTile);
    }

    updateDepthOfGobblers(x, z);

    if (oldTile == -1 || oldTile > MAX_TILE_INDEX) {
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
    timer->start(TIMER_VALUE);
}

void Mediator::afterAnimation() {
    for (const auto& item : std::as_const(m_list)) {
        if (item->depth() > 0 && item->isVisible()) {
            item->setVisible(false);
            item->update();
        }
    }

    // update state of new tile
    m_state[getSelection()->isWhite()][getSelection()->size()] |= (1 << myNewTile); //Set new position

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
        updateState(getTileX(newT), newY, getTileY(newT), move.from(), newT);
    }
}

void Mediator::resetItems(double width, double height) {
    for (const auto &item : std::as_const(m_list)) {
        item->setWidth(width);
        item->setHeight(height);
    }
}

void Mediator::updateGobbler() {
    m_comm->setLock(true);

    int x = getSelection()->x3d();
    int y = getSelection()->y3d();
    int z = getSelection()->z3d();

    if (newX != x) {
        if (newX < x) {
            x -= (x < (newX + SPEED)) ? (x - newX) : SPEED;
        } else {
            x += (x > (newX - SPEED)) ? (newX - x) : SPEED;
        }
    }

    if (newZ != z) {
        if (newZ < z) {
            z -= (z < (newZ + SPEED)) ? (z - newZ) : SPEED;
        } else {
            z += (z > (newZ - SPEED)) ? (newZ - z) : SPEED;
        }
    }


    if (x == newX && z == newZ) {
        y += (y > (newY - SPEED)) ? (newY - y) : SPEED;
    } else {
        if (y > -200) {
            y -= SPEED * 2;
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
    return MAX_TILE_INDEX - ((x + 225) / 150 + ((z + 225) / 150) * 4);
}

bool Mediator::isStack(double x){
    return isBlackTurn() ? x > 225 : x < -225;
}

bool Mediator::isNextToBoard(double x, double z, bool margin) {
    int test = 225 + (margin ? 75 : 0);
    return abs(x) > test || abs(z) > test;
}

int Mediator::getTileX(int tile){
    return 225 - 150 * (tile % 4);
}

int Mediator::getTileY(int tile){
    return 225 - 150 * (tile / 4);
}

int Mediator::roundCoord(double coord) {
    return ceil(coord / 150) * 150 - 75;
}

int Mediator::getBorderZ(int x, double y, int z) {
    int border;

    if (abs(x) > 225) { // is stack
        if (y > 75) { // outer circle
            border = 150;
        } else {
            if (y < -75) { // inner circle
                border = -150;
            } else {
                border = 0;
            }
        }
    } else {
        border = z;
    }
    return border;
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
        if ((WINNING_NO[i] & number) == WINNING_NO[i]) {
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

