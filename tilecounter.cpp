#include "tilecounter.h"

tileCounter::tileCounter(int tile, int whiteCounter, int blackCounter)
    : m_tile(tile), m_whiteCounter(whiteCounter), m_blackCounter(blackCounter) {};

int tileCounter::tile() const {
    return m_tile;
}
int tileCounter::whiteCounter() const {
    return m_whiteCounter;
}
int tileCounter::blackCounter() const {
    return m_blackCounter;
}

void tileCounter::addWhiteCounter(int value) {
    m_whiteCounter += value;
}
void tileCounter::addBlackCounter(int value) {
    m_blackCounter += value;
}
