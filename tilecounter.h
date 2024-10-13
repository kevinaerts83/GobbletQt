#ifndef TILECOUNTER_H
#define TILECOUNTER_H

class tileCounter
{
public:
    tileCounter(int tile, int whiteCounter = 0, int blackCounter = 0);

    int tile() const;

    int whiteCounter() const;
    void addWhiteCounter(int value);

    int blackCounter() const;
    void addBlackCounter(int value);
private:
    int m_tile;
    int m_whiteCounter;
    int m_blackCounter;
};

#endif // TILECOUNTER_H
