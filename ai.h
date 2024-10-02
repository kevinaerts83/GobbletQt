#ifndef AI_H
#define AI_H

#include <vector>
class AI
{

public:
    int m_fromTile = -1;
    int m_toTile = -1;

    AI();
    void move(int boardState [2][4]);


private:
    void cache (bool isWhite);

    bool tryToWin();
    bool dontLose();
    bool block();
    bool attack();
    void randomMove();

    void attackFallBack();
    void startAttack(bool ignore);
    int getNextMove(int mask, int state, bool reverse);
    bool setNextPawn(int size, std::vector<int> rows);

    std::vector<int> getRowsOfTile(int tile);
    int getPawnFromStack(int size);
    int getPawnFromBoard(int size, std::vector<int> excludeRows);
    std::vector<std::vector<int>> getUnimportantWhiteTile(int size, std::vector<int> excludeRows);

    int getObjectFromArray(int objects [], int value);
    void getColorUnderneath(int tile, int size, int bState);
    void getTilesOfSize(int size);
    int getPawnSize(int tile);
    void sortWhiteRows(int bState);
    void chooseTileFromRowToAttack(int bState, int rowToAttack);
    std::vector<int> rowCheck(int maxCount, bool isWhite, bool removeRowsWithSize0);
    void getSmallestBlackPawnOfRow(int mask);
    int count1Bits(int x);

    void writeLog();

    int m_mask [10] = { 61440, 3840, 240, 15, 34952, 17476, 8738, 4369, 33825, 4680 };

    int m_bState [2][4];
    int m_level = 0;
    int m_tiles [16] = { 0, 1, 2, 3, 5, 6, 7, 8, 10, 11, 12, 13, 15, 16, 17, 18 };
    int m_visibleWhite = 0;
    int m_visibleBlack = 0;
    int m_visibleWhiteRows [4] = { 0, 0, 0, 0 };
    int m_visibleBlackRows [4] = { 0, 0, 0, 0 };
    //bool m_moving = false;
    //int m_report = -1;
    int m_crossings = 38505;

    int m_randomMaskSequence [10];

};

#endif // AI_H
