#ifndef AI_H
#define AI_H

#include <vector>
#include "aimove.h"

class AI
{

public:
    AI(int level);
    aiMove move(int boardState [2][4]);
    bool validateStackMove(int boardState [2][4], bool isWhite, int toTile);

private:
    int m_level;

    void init(int boardState [2][4]);

    void setVisibleWhite();
    void setVisibleBlack();
    void getVisiblePawns(int main, int temp [4]);

    void tryToWin(aiMove& move);
    void dontLose(aiMove& move);
    void dontLoseCrossing(std::vector<int>& rows, aiMove& move);
    void dontLoseBlock(std::vector<int>& rows, aiMove& move);
    void blockFutureCrossing(aiMove& move);
    void attack(aiMove& move);
    void randomMove(aiMove& move);
    void attackFallBack(aiMove& move);

    int getNewToTile(int ignore);
    int getTileFromRowUnderAttack(int rowToAttack);

    int getToTile(int mask, int visiblePawns);
    int getUnOccupiedTile(int mask, int visiblePawns);
    int getFromTile(int moveTo);
    int getFromTile(int moveTo, int size, std::vector<int> excludeRows);

    std::vector<int> getRowsOfTile(int tile);
    int getPawnFromStack(int size);
    int getPawnFromBoard(int size, std::vector<int> excludeRows);
    int getPawnFromBoardEasy(int size, std::vector<int> excludeRows);
    int getPawnFromBoardHard(int size, std::vector<int> excludeRows);
    int getFromBoard(int size, int tile);

    bool findId(std::vector<int> numbers, int searchId);
    int getColorUnderneath(int tile, int size);
    std::vector<int> getBlackTilesOfSize(int size);
    int getPawnSize(int tile);
    std::vector<std::pair<int, int>> sortWhiteRows();

    int getSmallestWhiteOfRow(int mask);
    int count1Bits(int x);
    int countrZero(int n);
    int tileToNumber(int tile);
    int getRandom(int upperBound);

    std::vector<int> rowCheck(int maxCount, bool isWhite, bool removeRowsWithSize0);
    std::vector<int> findRowsBlackWins();
    std::vector<int> findRowsWhiteWins();
    std::vector<int> findRowsWhiteCouldWin();

    void writeLog();

    int m_bState [2][4];
    int m_visibleWhite = 0;
    int m_visibleBlack = 0;
    int m_visibleWhiteRows [4] = { 0, 0, 0, 0 };
    int m_visibleBlackRows [4] = { 0, 0, 0, 0 };

    int m_randomMaskSequence [10];

    enum Size {
        Huge,
        Large,
        Medium,
        Small,
        Empty
    };

    enum Player {
        Black,
        White
    };

    enum Mode {
        OneVsOne,
        Easy,
        Hard
    };

    const int MASKS [10] = { 61440, 3840, 240, 15, 34952, 17476, 8738, 4369, 33825, 4680 };
    const int TWICE_THREE [32] = { 63624, 62532, 61986, 61713, 36744, 20292, 12066, 7953, 35064, 17652, 8946, 4593, 34959, 17487, 8751, 4383, 62497, 36641, 34033, 33839, 62024, 8008, 4856, 4687, 36009, 50277, 42531, 38193, 39624, 22092, 12906, 4953 };
    const int BLOCK_TWICE_THREE [32] = { 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 32768, 1024, 32, 1, 4096, 512, 64, 8, 32768, 1024, 32, 1, 8, 64, 512, 4096 };

    static const int CROSSING = 38505;
    static const int MAX_ROWS = 10;
    static const int MAX_TILES = 16;
    static const int ROW_SIZE = 4;

    struct GradedTile {
        int tile;
        int size;
        int whiteGobblers = 0;
        int blackGobblers = 0;
        int colorUnderneath;
        int grade;
    };
};

#endif // AI_H
