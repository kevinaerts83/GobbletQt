#include "ai.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <bitset>
#include <chrono>

AI::AI(int level) {
    m_level = level;
    std::vector<int> sequence { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sequence.begin(), sequence.end(), std::default_random_engine(seed));

    for (int i = 0; i < 10; i++) {
        m_randomMaskSequence[i] = sequence[i];
    }
}

// Move function
aiMove AI::move(int boardState [2][4]) {

    init(boardState);

    aiMove move;
    tryToWin(move);
    if (move.validate()) return move;
    dontLose(move);
    if (move.validate()) return move;
    blockFutureCrossing(move);
    if (move.validate()) return move;
    attack(move);
    return move;
}

bool AI::validateStackMove(int boardState [2][4], bool isWhite, int toTile) {
    init(boardState);

    std::vector<int> allowedRows = rowCheck(3, isWhite, true);
    std::vector<int> rowsOfTile = getRowsOfTile(toTile);

    return std::any_of(
        allowedRows.begin(),
        allowedRows.end(),
        [&](int val) {
            return std::find(rowsOfTile.begin(), rowsOfTile.end(), val) != rowsOfTile.end();
        });
}

void AI::init(int boardState [2][4]) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            m_bState[i][j] = boardState[i][j];
        }
    }

    setVisibleWhite();
    setVisibleBlack();
}

// Try to win
void AI::tryToWin(aiMove& move) {
    std::vector<int> rows = findRowsBlackWins();
    for (int row : rows) {
        int maskToCheck = MASKS[row];
        move.setTo(getUnOccupiedTile(maskToCheck, m_visibleBlack));

        int size = (tileToNumber(move.to()) & m_visibleWhite) != 0 ? getPawnSize(move.to()) : Size::Empty;
        move.setFrom(getFromTile(move.to(), size, {row}));
        if (move.from() > -1) {
            break;
        }
    }
}

// Dont lose
void AI::dontLose(aiMove& move) {
    std::vector<int> rows = findRowsWhiteWins();
    if (rows.size() > 1) {
        dontLoseCrossing(rows, move);
    } else if (rows.size() == 1) {
        dontLoseBlock(rows, move);
    }
}

void AI::dontLoseCrossing(std::vector<int>& rows, aiMove& move) {
    int maskCrossing = MASKS[rows[0]];
    for (int row : rows) {
        maskCrossing &= MASKS[row]; // row 0 is taken twice; but it's oke
    }
    if (maskCrossing > 0) {
        move.setTo(countrZero(maskCrossing));
        if (getPawnSize(move.to()) != 0) {
            // if large; only a huge gobbler will be picked; exclude rows with 3 whites
            move.setFrom(getFromTile(move.to(), Size::Large, findRowsWhiteCouldWin()));
        }
    }
    if (move.from() == -1) {
        // no possibility to block white do random move
        randomMove(move);
    }
}

void AI::dontLoseBlock(std::vector<int>& rows, aiMove& move) {
    int maskToCheck = MASKS[rows[0]];
    int s = getSmallestWhiteOfRow(MASKS[rows[0]]);
    if (s == Size::Huge) { // all 3 huge pawns
        move.setTo(getUnOccupiedTile(maskToCheck, m_visibleWhite));
        move.setFrom(getFromTile(move.to(), Size::Large, rows)); // dontLose function already checked no Huge black pawn is here
    } else {
        move.setTo(getToTile(maskToCheck, m_visibleWhiteRows[s])); // get tile with small white
        move.setFrom(getFromTile(move.to(), s, rows));
    }
}

// Check if white player can put a huge pawn on a tile creating 2 row with 3 pawns.
void AI::blockFutureCrossing(aiMove& move) {
    if (m_level == Mode::Hard) {
        int i = 0;
        for (int twiceThree : TWICE_THREE) {
            int block = BLOCK_TWICE_THREE[i];
            if ((m_bState[Player::Black][Size::Huge] & block) > 0) {
                    continue; // already blocked
            }
            /* 1 = whites, B is the empty block move
             * 0 1 0 0
             * 1 B 1 0
             * 0 0 0 0
             * 0 1 0 0
             */
            int nrWhites = count1Bits(m_visibleWhite & twiceThree);
            int nrHugeWhites = count1Bits(m_bState[Player::White][Size::Huge] & twiceThree);
            int nrHugeBlackes = count1Bits(m_bState[Player::Black][Size::Huge] & twiceThree);

            if (nrWhites >= 4 && nrHugeWhites < 3 && nrHugeBlackes == 0) {
                // fake a large white pawn on the blocking tile and execute don't loose

                int previousBoardState = m_bState[Player::White][Size::Large];
                int previousVisibleWhite = m_visibleWhite;
                int previousVisibleLarge = m_visibleWhiteRows[Size::Large];

                m_bState[1][1] |= block;
                m_visibleWhite |= block;
                m_visibleWhiteRows[1] |= block;

                dontLose(move);

                m_bState[1][1] = previousBoardState;
                m_visibleWhite = previousVisibleWhite;
                m_visibleWhiteRows[1] = previousVisibleLarge;
                break;
            }
            i++;
        }
    }
}

void AI::attack(aiMove& move) {
    std::vector<std::pair<int, int>> wRows = sortWhiteRows();
    for (std::pair<int, int>& row : wRows) {
        move.setTo(getTileFromRowUnderAttack(row.first)); // row.first == row id

        if (move.to() >= 0) {
            move.setFrom(getFromTile(move.to()));
            if (move.from() > -1) {
                break;
            }
        }
    }

    if (move.to() < 0) {
        attackFallBack(move);
    }

    if (!move.validate()) { // no pawn or tile found
        //writeLog(bState);
        randomMove(move);
        //std::cout << "current tile: " << nextMove << std::endl;
    }
}

void AI::randomMove(aiMove& move) {
    int s = Size::Huge;
    int random = getRandom(MAX_TILES);
    int tile = -1;
    for (int i = 0; i < MAX_TILES && s == Size::Huge; i++) {
        tile = random++ % MAX_TILES;
        if (m_level < Mode::Hard || ((tile & CROSSING) > 0 && (getColorUnderneath(tile, -1) != Player::Black))) {
            s = getPawnSize(tile);
            break;
        }
    }
    move.setTo(tile);
    move.setFrom(getPawnFromStack(s - 1));
    if (move.from() < MAX_TILES) {
        int numTile = tileToNumber(move.to());
        move.setFrom(getFromBoard(s, numTile));
    }
}

void AI::attackFallBack(aiMove& move) {
    int ignore = 0;
    for (int i = 0; i < ROW_SIZE; i += 1) {
        if (getFromTile(move.to()) == -1) {
            ignore += tileToNumber(move.to());
            move.setTo(getNewToTile(ignore));
        } else {
            break;
        }
    }
}

int AI::getNewToTile(int ignore) {
    int num = CROSSING ^ m_bState[Player::White][Size::Huge] ^ m_bState[Player::Black][Size::Huge] ^ ignore;
    int tile = countrZero(num);
    return tile > -1 ? tileToNumber(tile) : -1;
}

int AI::getTileFromRowUnderAttack(int rowToAttack) {
    if (rowToAttack == -1) {
        //do random move
        return getNewToTile(0);
    } else {
        int rowMask = MASKS[rowToAttack];
        int cTiles = (rowMask & (m_visibleWhite | m_visibleBlack)) ^ rowMask;
        if (cTiles == 0) {
            int s = getSmallestWhiteOfRow(rowMask);
            //cTiles = array of tiles with the smallest black pawns
            cTiles = m_bState[0][s];
            for (s -= 1; s > -1; s -= 1) {
                cTiles = (cTiles ^ (m_bState[0][s] | m_bState[1][s])) & cTiles;
            }
        }
        if (m_level == Mode::Hard && ((cTiles & CROSSING) > 0)) {
            cTiles &= CROSSING;
        }
        return getToTile(rowMask, cTiles);
    }
}

int AI::getToTile(int mask, int visiblePawns) {
    return countrZero(mask & visiblePawns);
}

int AI::getUnOccupiedTile(int mask, int visiblePawns) {
    return countrZero((mask & visiblePawns) ^ mask);
}

int AI::getFromTile(int moveTo) {
    int size = getPawnSize(moveTo);
    int stack = size == Size::Empty ? getPawnFromStack(size) : -1;
    if (stack >= MAX_TILES) {
        return stack;
    } else {
        std::vector<int> excludeRows = getRowsOfTile(moveTo);
        return getPawnFromBoard(size, excludeRows);
    }
    return -1;
}

int AI::getFromTile(int moveTo, int size, std::vector<int> excludeRows) {
    int stack = getPawnSize(moveTo) == Size::Empty ? getPawnFromStack(size) : -1;
    if (stack >= MAX_TILES) {
        return stack;
    } else {
        return getPawnFromBoard(size, excludeRows);
    }
    return -1;
}

std::vector<int> AI::getRowsOfTile(int tile) {
    std::vector<int> rows;
    int tileNo = tileToNumber(tile);
    for (int i = 0; i < MAX_ROWS; i++) {
        if ((MASKS[i] & tileNo) != 0) {
            rows.push_back(i);
        }
    }
    return rows;
}

/* find smallest pawn but bigger as size and not on board */
int AI::getPawnFromStack(int size) {
    int maxNo = 3;
    int pawnSize = -1;
    for (int i = 0; i < size; i++) {
        int onBoard = count1Bits(m_bState[Player::Black][i]);
        if (onBoard < maxNo) {
            pawnSize = i;
        }
        maxNo = onBoard;
    }
    return MAX_TILES + pawnSize;
}

int AI::getPawnFromBoard(int size, std::vector<int> excludeRows) {
    if (m_level == Mode::Easy) {
        return getPawnFromBoardEasy(size, excludeRows);
    } else {
        return getPawnFromBoardHard(size, excludeRows);
    }
}

int AI::getPawnFromBoardEasy(int size, std::vector<int> excludeRows) {
    // get pawn from board which pawnSize < size
    std::vector<int> whiteWins = findRowsWhiteCouldWin();
    for (int i = size - 1; i >= Size::Huge; i--) {
        int pawns = m_visibleBlackRows[i];

        // exclude: white wins rows
        for (int ww : whiteWins) {
            pawns &= ~MASKS[ww]; // NAND == and not
        }
        // exclude: exclude rows
        for (int exc : excludeRows) {
            pawns &= ~MASKS[exc];
        }
        // return: tile
        return countrZero(pawns);
    }
    return -1;
}

int AI::getPawnFromBoardHard(int size, std::vector<int> excludeRows) {

    std::vector<int> rowsWhiteWins = findRowsWhiteCouldWin();
    std::vector<GradedTile> gradedTiles;

    for (int s = size - 1; s >= Size::Huge; s--) {

        std::vector<int> bTiles = getBlackTilesOfSize(s);
        for (int aTile : bTiles) {

            GradedTile gt;
            gt.tile = aTile;
            gt.size = s;
            gt.colorUnderneath = getColorUnderneath(aTile, s);
            bool valid = true;

            int nrTile = tileToNumber(aTile);
            std::vector<int> rows = getRowsOfTile(aTile);
            for (int r : rows) {

                //stop processing the tile if it's in a row with 3 whites, or it's the current row
                if (findId(rowsWhiteWins, r) || findId(excludeRows, r)) {

                    valid = false;
                    break;
                }

                // stop if there are already 2 white gobblers in the row
                // and underneath the black is a third white pawn
                // and no black Huge to block it.
                if ((count1Bits(MASKS[r] & m_visibleWhite) == 2) &&
                    (gt.colorUnderneath == Player::White) &&
                    ((((MASKS[r] & m_bState[Player::Black][Size::Huge]) | nrTile) ^ nrTile) == 0)) {

                    valid = false;
                    break;
                }

                gt.whiteGobblers += count1Bits(m_visibleWhite & MASKS[r]);
                gt.blackGobblers += count1Bits(m_visibleBlack & MASKS[r]);
            }

            if (valid) {
                gradedTiles.push_back(gt);
            }
        }
    }

    for (GradedTile gt : gradedTiles) {
        // the best/biggest pawn we do not want to move
        gt.grade = ((4 - gt.size) * 2) +
                   gt.whiteGobblers +
                   (gt.blackGobblers * 2) +
                   ((gt.colorUnderneath == -1) ? 0 : (gt.colorUnderneath == Player::White) ? -2 : 5);
    }

    auto maxId = std::max_element(gradedTiles.begin(), gradedTiles.end(),
                                  [](const GradedTile& a, const GradedTile& b) {
                                      return a.grade < b.grade;
                                  });

    return (maxId != gradedTiles.end()) ? maxId->tile : getPawnFromBoardEasy(size, excludeRows);
}

int AI::getFromBoard(int size, int tile) {
    for (int i = size - 1; i >= Size::Huge; i--) {
        int possibleTiles = m_visibleBlackRows[i] & ~(tile);
        if (possibleTiles > 0) {
            return countrZero(possibleTiles);
        }
    }
    return -1;
}

void AI::setVisibleWhite() {
    int temp [ROW_SIZE];
    getVisiblePawns(Player::White, temp);
    m_visibleWhite = temp[Size::Huge] | temp[Size::Large] | temp[Size::Medium] | temp[Size::Small];
    for (int s = Size::Huge; s < Size::Empty; s++) {
        m_visibleWhiteRows[s] = temp[s];
    }
}

void AI::setVisibleBlack() {
    int temp [ROW_SIZE];
    getVisiblePawns(Player::Black, temp);
    m_visibleBlack = temp[Size::Huge] | temp[Size::Large] | temp[Size::Medium] | temp[Size::Small];
    for (int s = Size::Huge; s < Size::Empty; s++) {
        m_visibleBlackRows[s] = temp[s];
    }
}

void AI::getVisiblePawns(int main, int temp [4]) {
    int other = !main;

    temp[Size::Huge] = m_bState[main][Size::Huge];
    int onTop = m_bState[other][Size::Huge];
    temp[Size::Large] = (m_bState[main][Size::Large] ^ onTop) & m_bState[main][Size::Large];
    onTop |= m_bState[other][Size::Large];
    temp[Size::Medium] = (m_bState[main][Size::Medium] ^ onTop) & m_bState[main][Size::Medium];
    onTop |= m_bState[other][Size::Medium];
    temp[Size::Small] = (m_bState[main][Size::Small] ^ onTop) & m_bState[main][Size::Small];
}

bool AI::findId(std::vector<int> numbers, int searchId) {
    return std::find(numbers.begin(), numbers.end(), searchId) != numbers.end();
}

int AI::getColorUnderneath(int tile, int size) {
    int number = tileToNumber(tile);
    for (int s = size + 1; s < Size::Empty; s++) {
        if ((m_bState[Player::White][s] & number) != 0) {
            return Player::White;
        }
        if ((m_bState[Player::Black][s] & number) != 0) {
            return Player::Black;
        }
    }
    return -1;
}

std::vector<int> AI::getBlackTilesOfSize(int size) {
    std::vector<int> bTiles;

    int number = m_visibleBlackRows[size];
    for (int bitIndex = 0; number != 0; bitIndex++) {
        if (number & 1) {  // Check if the least significant bit is 1
            bTiles.push_back(bitIndex);
        }
        number >>= 1;  // Shift the number to the right by 1 bit
    }
    return bTiles;
}

int AI::getPawnSize(int tile) {
    int mask = tileToNumber(tile);
    for (int s = Size::Huge; s < Size::Empty; s++) {
        if (((m_bState[Player::White][s] | m_bState[Player::Black][s]) & mask) > 0) {
            return s;
        }
    }
    return Size::Empty;
}

std::vector<std::pair<int, int>> AI::sortWhiteRows() {
    std::vector<std::pair<int, int>> whiteBestRows;
    for (int i = 0; i < MAX_ROWS; i++) {
        int j = m_randomMaskSequence[i]; // shuffled numbers from 0 to 10
        int aMask = MASKS[j];
        if (this->count1Bits(aMask & (m_bState[Player::Black][Size::Huge] | m_bState[Player::White][Size::Huge])) != ROW_SIZE) {
            int countRow = count1Bits(m_visibleBlack & aMask) * 5; // number of tiles on row occupied by black * 5
            countRow += count1Bits(m_visibleWhiteRows[Size::Small] & aMask) * 4; // tiles occupied by smallest white * 4
            countRow += count1Bits((aMask & (m_visibleWhite | m_visibleBlack)) ^ aMask) * 3; //empty tiles
            countRow += count1Bits(m_visibleWhiteRows[Size::Medium] & aMask) * 3;
            countRow += count1Bits(m_visibleWhiteRows[Size::Large] & aMask) * 2;
            countRow += count1Bits(m_visibleWhiteRows[Size::Huge] & aMask);

            whiteBestRows.push_back({j, countRow});
        } else {
            whiteBestRows.push_back({j, 0});
        }
    }

    std::sort(whiteBestRows.begin(), whiteBestRows.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    return whiteBestRows;
}

int AI::getSmallestWhiteOfRow(int mask) {
    for (int i = Size::Small; i >= Size::Huge; i--) {
        if ((mask & m_visibleWhiteRows[i]) != 0) {
            return i;
        }
    }
    return Size::Empty;
}

// equals to popcount; c++ 20
int AI::count1Bits(int x) {
    //return std::bitset<16>(x).count();
    int count = 0;
    while (x) {
        x &= x - 1;
        count++;
    }
    return count;
}

// equals to std::countr_zero(x); C++20
// count number of zero bits before the first 1
int AI::countrZero(int num) {
    int bitCount = count1Bits(num);

    if (bitCount == 0) {
        return -1; // No 1 bits, return 0
    }

    // Generate a random number between 1 and bitCount
    int randomBitIndex = getRandom(bitCount - 1) + 1; // 0 is not allowed

    // Locate the chosen 1 bit
    int currentBitIndex = 0;
    for (int t = 0; t < MAX_TILES; t++) {
        if (num & tileToNumber(t)) { // Check if the i-th bit is 1
            ++currentBitIndex;
            if (currentBitIndex == randomBitIndex) {
                return t;
            }
        }
    }

    return -1; // Impossible to reach this line
}

int AI::getRandom(int upperBound) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, upperBound);
    return dis(gen);
}

int AI::tileToNumber(int tile) {
    return 1 << tile; // 2 ^ tile
}

std::vector<int> AI::findRowsBlackWins() {
    return rowCheck(3, false, true);
}

std::vector<int> AI::findRowsWhiteWins() {
    return rowCheck(3, true, true);
}

std::vector<int> AI::findRowsWhiteCouldWin() {
    return rowCheck(3, true, false);
}

/**
 * Find rows with the maxCount of gobblers
 */
std::vector<int> AI::rowCheck(int maxCount, bool isWhite, bool removeRowsWithSize0) {
    std::vector<int> rows;
    int number = (isWhite ? m_visibleWhite : m_visibleBlack);
    for (int i = 0; i < MAX_ROWS; i++) {
        if (this->count1Bits(MASKS[i] & number) == maxCount) {
            if (removeRowsWithSize0) {
                if ((m_bState[!isWhite][Size::Huge] & MASKS[i]) == 0) {
                    rows.push_back(i);
                }
            } else {
                rows.push_back(i);
            }
        }
    }
    return rows;
}

// Write log (simulating the console.log in C++)
void AI::writeLog() {
    auto toBinaryString = [](int n) {
        return std::bitset<16>(n).to_string();
    };
    std::cout << "black" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << toBinaryString(m_bState[0][i]) << std::endl;
    }
    std::cout << "white" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << toBinaryString(m_bState[1][i]) << std::endl;
    }
}
