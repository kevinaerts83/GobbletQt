#ifndef AI_H
#define AI_H

#include <vector>
#include "grades.h"
#include "tilecounter.h"
#include "aimove.h"

class AI
{

public:
    int m_fromTile = -1;
    int m_toTile = -1;

    AI();
    aiMove move(int boardState [2][4]);


private:
    void cache (bool isWhite);

    bool tryToWin();
    bool dontLose();
    bool block();
    bool attack();
    void randomMove();

    void attackFallBack();
    int startAttack(int ignore);
    int getToTile(int mask, int state, bool reverse);
    bool setFromTile(int size, std::vector<int> rows);

    std::vector<int> getRowsOfTile(int tile);
    int getPawnFromStack(int size);
    int getPawnFromBoard(int size, std::vector<int> excludeRows);
    std::vector<Grades> getUnimportantWhiteTile(int size, std::vector<int> excludeRows);

    bool findId(std::vector<int> numbers, int searchId);
    tileCounter* getObjectFromArray(std::vector<tileCounter>& counters, int tile);
    int getColorUnderneath(int tile, int size);
    std::vector<int> getTilesOfSize(int size);
    int getPawnSize(int tile);
    std::vector<Grades> sortWhiteRows();
    void chooseTileFromRowToAttack(int rowToAttack);
    std::vector<int> rowCheck(int maxCount, bool isWhite, bool removeRowsWithSize0);
    int getSmallestWhiteOfRow(int mask);
    int count1Bits(int x);
    int get_first_set_bit_position(int n);

    void writeLog();

    int m_mask [10] = { 61440, 3840, 240, 15, 34952, 17476, 8738, 4369, 33825, 4680 };
    int m_twiceThree [32] = { 63624, 62532, 61986, 61713, 36744, 20292, 12066, 7953, 35064, 17652, 8946, 4593, 34959, 17487, 8751, 4383, 62497, 36641, 34033, 33839, 62024, 8008, 4856, 4687, 36009, 50277, 42531, 38193, 39624, 22092, 12906, 4953 };
    int m_blockMove [32] = { 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 32768, 1024, 32, 1, 4096, 512, 64, 8, 32768, 1024, 32, 1, 8, 64, 512, 4096 };
    int m_crossings = 38505;

    int m_bState [2][4];
    int m_level = 0;
    int m_visibleWhite = 0;
    int m_visibleBlack = 0;
    int m_visibleWhiteRows [4] = { 0, 0, 0, 0 };
    int m_visibleBlackRows [4] = { 0, 0, 0, 0 };

    int m_randomMaskSequence [10];
};

#endif // AI_H
