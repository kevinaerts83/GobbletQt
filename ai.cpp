#include "AI.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <bitset>

AI::AI() {
    std::array<int, 10> sequence { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sequence.begin(), sequence.end(), std::default_random_engine(seed));

    for (int i = 0; i < 10; i++) {
        m_randomMaskSequence[i] = sequence[i];
    }
}

// Move function
void AI::move(int boardState [2][4]) {

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            m_bState[i][j] = boardState[i][j];
        }
    }

    cache(0);
    cache(1);

    //m_report = 0;

    if (tryToWin()) {
        //m_report = 1;
        if (dontLose()) {
            //m_report = 2;
            if (block()) {
                //m_report = 3;
                attack();
            }
        }
    }

    if (false) { // no pawn or tile found
        //writeLog(bState);
        randomMove();
        //std::cout << "current tile: " << nextMove << std::endl;
    }
}

// Cache function
void AI::cache(bool isWhite) {
    int w = isWhite;
    int b = !isWhite;

    int temp [4];
    temp[0] = m_bState[w][0];
    int onTop = m_bState[b][0];
    temp[1] = (m_bState[w][1] ^ onTop) & m_bState[w][1];
    onTop |= m_bState[b][1];
    temp[2] = (m_bState[w][2] ^ onTop) & m_bState[w][2];
    onTop |= m_bState[b][2];
    temp[3] = (m_bState[w][3] ^ onTop) & m_bState[w][3];
    int t = temp[0] | temp[1] | temp[2] | temp[3];

    if (isWhite) {
        m_visibleWhite = t;
        for (int i = 0; i < 4; i++) {
            m_visibleWhiteRows[i] = temp[i];
        }
    } else {
        m_visibleBlack = t;
        for (int i = 0; i < 4; i++) {
            m_visibleBlackRows[i] = temp[i];
        }
    }
}

// Try to win
bool AI::tryToWin() {
    std::vector<int> rows = rowCheck(3, true, true);
    for (int row : rows) {
        int maskToCheck = m_mask[row];
        m_toTile = getNextMove(maskToCheck, m_visibleWhite, true);
        if ((m_visibleBlack & maskToCheck) != 0) {
            int blackSize = getPawnSize(m_toTile);
            if (blackSize > 0 && blackSize < 4) {
                return setNextPawn(blackSize, rows);
            }
        } else {
            return setNextPawn(4, rows);
        }
    }
    return true;
}

// Dont lose
bool AI::dontLose() {
    std::vector<int> rows = rowCheck(3, false, false);
    if (rows.size() > 1) {
        int maskCrossing = m_mask[rows[0]];
        for (int i = 1; i < static_cast<int>(rows.size()); ++i) {
            maskCrossing &= m_mask[rows[i]];
        }
        if (maskCrossing > 0) {
            m_toTile = getNextMove(0, maskCrossing, false);
            if (getPawnSize(m_toTile) != 0) {
                if (setNextPawn(1, rows)) {
                    return false;
                }
            }
        }
    } else if (rows.size() == 1) {
        int maskToCheck = m_mask[rows[0]];
        int s = getSmallestBlackPawnOfRow(m_mask[rows[0]]);
        m_toTile = getNextMove(maskToCheck, m_visibleBlackRows[s], false);
        if (setNextPawn(s, rows)) {
            return false;
        }
        m_toTile = getNextMove(maskToCheck, m_visibleBlack, true);
        if (setNextPawn(1, rows)) {
            return false;
        }
    }
    return true;
}

bool AI::block() {
    //if level is 1: check if black player can put a size0 on a tile creating 2 row with 3 pawns. (no biggest white pawns on the rows)
    //fake a black pawn size1 on that tile and do don't loose
    if (m_level == 1) {
        bool ret = true;

        int i = 0;
        for (int twiceThree : m_twiceThree) {
            if ((count1Bits(m_visibleBlack & twiceThree) > 3)
                && ((m_visibleBlack & m_blockMove[i]) == 0)
                && ((m_bState[1][0] & twiceThree) == 0)) {

                int oldValue = m_bState[0][1];
                int old2 = m_visibleBlack;
                int old3 = m_visibleBlackRows[1];

                m_bState[0][1] |= m_blockMove[i];
                m_visibleBlack |= m_blockMove[i];
                m_visibleBlackRows[1] |= m_blockMove[i];

                ret = dontLose();

                m_bState[0][1] = oldValue;
                m_visibleBlack = old2;
                m_visibleBlackRows[1] = old3;
                break;
            }
            i++;
        }
        return ret;
    }
    return false;
}

bool AI::attack() {
    std::vector<Grades> wRows;

    if (m_visibleWhite != 0) {
        //determine whites best row lower is better
        wRows = sortWhiteRows();
    }
    for (int i = 0; i < 10; i += 1) {
        int rowToAttack = (static_cast<int>(wRows.size()) > i) ? wRows[i].id : -1;
        //set the nextMove
        chooseTileFromRowToAttack(rowToAttack);

        if (m_toTile >= 0) {
            int s = getPawnSize(m_toTile);
            s = (s == -1 ? 4 : s);
            if (setNextPawn(s, getRowsOfTile(m_toTile))) {
                break;
            }
        }
        if (static_cast<int>(wRows.size()) < i) {
            break;
        }
    }

    if (m_toTile < 0) {
        attackFallBack();
    }
    return true;
}

void AI::randomMove() {
    int s = 0;
    while (s == 0) {
        int t = std::rand() % 16;
        m_toTile = t;
        s = getPawnSize(m_toTile);
    }

    int stackSize = getPawnFromStack(s - 1);
    if (stackSize > -1) {
        m_fromTile = 16 + stackSize;
    }

    for (int i = s - 1; i > 0; i--) {
        int pawnsOnBoard = m_visibleBlackRows[i];
        int toTile = pow(2, m_toTile);
        int possibleTiles = pawnsOnBoard ^ toTile;
        if (possibleTiles > 0) {
            m_fromTile = possibleTiles & -possibleTiles;
        }
    }
}

void AI::attackFallBack() {
    int ignore = 0;
    for (int i = 0; i < 4; i += 1) {
        int size = getPawnSize(m_toTile);
        size = (size == -1 ? 4 : size);
        if (!setNextPawn(size, getRowsOfTile(m_toTile))) {
            m_toTile = startAttack(ignore);
            ignore += pow(2, m_toTile);
        } else {
            break;
        }
    }
}

int AI::startAttack(int ignore) {
    int number = m_crossings ^ m_bState[0][0] ^ m_bState[1][0] ^ ignore;
    // Convert number to binary string
    std::string binary = std::bitset<32>(number).to_string(); // Assuming a 32-bit number

    // Remove leading zeros from the binary string
    binary = binary.substr(binary.find('1'));

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, binary.length() - 1);

    int rnd, t;

    // Get random index and find first '1' after that index
    do {
        rnd = dist(gen);  // Generate a random number
        t = binary.find('1', rnd);  // Find the first '1' starting from rnd
    } while (t == std::string::npos);  // Repeat if '1' is not found

    return t;
}



// { 0, 1, 2, 3, 5, 6, 7, 8, 10, 11, 12, 13, 15, 16, 17, 18 }
// 0000 1111 0000 0000 rowMask
// 1100 0001 0000 0100 visiblePawns

// 1 0000 0000 = 9
// tiles [9 - 1] = 8
int AI::getNextMove(int rowMask, int visiblePawns, bool reverse) {
    int result = (rowMask == 0) ? visiblePawns : (rowMask & visiblePawns);
    if (rowMask != 0 && reverse) {
        result ^= rowMask;
    }
    return m_tiles[std::bitset<16>(result).count() - 1];
}

bool AI::setNextPawn(int size, std::vector<int> rows) {
    bool ret = false;
    int solution = getPawnFromStack(size);
    if (solution > 0) {
        m_fromTile = solution;
        ret = true;
    } else {
        solution = getPawnFromBoard(size, rows);
        if (solution > -1) {
            m_fromTile = solution;
            ret = true;
        }
    }
    return ret;
}

std::vector<int> AI::getRowsOfTile(int tile) {
    std::vector<int> rows;
    int tileNo = pow(2, tile); // iso tile tiles[tile].index
    for (int i = 0; i < 10; i += 1) {
        if ((m_mask[i] & tileNo) != 0) {
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
        int onBoard = count1Bits(m_bState[0][i]); // 0 means it's white turn
        if (onBoard < maxNo) {
            pawnSize = i;
        }
        maxNo = onBoard;
    }
    return pawnSize;
}

int AI::getPawnFromBoard(int size, std::vector<int> excludeRows) {
    if (m_level == 0) {
        // get pawn from board which pawnSize < size
        for (int i = size - 1; i >= 0; i--) {
            int pawns = m_visibleWhiteRows[i];

            while (pawns > 0) {
                int binairTile = pow(2, std::bitset<16>(pawns).count() - 1);
                pawns -= binairTile;

                for (int j = 0; j < 10; j++) {
                    if ((m_mask[j] & binairTile) == binairTile) {
                        std::vector<int> otherPlayerWins = rowCheck(3, true, false);
                        if (std::find(otherPlayerWins.begin(), otherPlayerWins.end(), j) != otherPlayerWins.end()
                            && std::find(excludeRows.begin(), excludeRows.end(), j) != excludeRows.end()) {
                            return binairTile;
                        }
                    }
                }
            }
        }
    } else {
        int result = -1;
        int j = 0;
        for (int i = size - 1; i > -1; i -= 1) {
            std::vector<Grades> graded = getUnimportantWhiteTile(i, excludeRows);
            for (Grades grade : graded) {
                if ((j == 0) || (j > grade.id)) {
                    j = grade.id;
                    result = grade.grade;
                }
            }
        }
        if (result > 0) {
            return result;
        }

        for (int i = size - 1; i >= 0; i--) {
            int pawns = m_visibleWhiteRows[i];

            if (pawns > 0) {
                return pow(2, std::bitset<16>(pawns).count() - 1);
            }
        }
    }
    return -1;
}

std::vector<Grades> AI::getUnimportantWhiteTile(int size, std::vector<int> excludeRows) {
    std::vector<int> wTiles = getTilesOfSize(size); //get the tile of a visible white pawns from a certain size
    std::vector<int> goodTiles;

    for (int aTile : wTiles) {
        bool bad = false;
        //tileRows contains the row indexes, these indexes are the same as the mask indexes for those rows
        std::vector<int> tileRows = getRowsOfTile(aTile);
        //loop over every row of the tile (max 3)
        //a white pawn is only unimportant if it's not blocking a row or not of strategic interest for a row
        for (int r : tileRows) {
            int p = pow(2, aTile);
            //stop processing the tile if it's in a row with 3 black pawns, or the row that is currently formed.
            if (findId(rowCheck(3, false, false), r) || findId(excludeRows, r)) {
                bad = true;
                break;
            } else if ((count1Bits(m_mask[r] & m_visibleBlack) == 2) &&
                       ((((m_mask[r] & m_bState[1][0]) | p) ^ p) == 0) &&
                       (getColorUnderneath(aTile, size) == 2)) {
                //Stop when all conditions are true
                // - 2 black pawn
                // - no biggest white pawn (except the one being checked)
                // - and one black pawn underneath (the one being checked)
                bad = true;
                break;
            } else {
                //Set cache for Grade
                //Count for every row the tile is in, the white and black pawns.
                o = getObjectFromArray(cache, aTile);//id === tile
                if (o) {
                    o.countWhite += count1Bits((m_visibleWhite & m_mask[r]));
                    o.countBlack += count1Bits((m_visibleBlack & m_mask[r]));
                } else {
                    cache.push({id: aTile,
                                countWhite: count1Bits((m_visibleWhite & m_mask[r])),
                                countBlack: count1Bits((m_visibleBlack & m_mask[r]))});
                }
            }
        }
        if (!bad) {
            //initialize result array for tiles that passed the test.
            goodTiles.push_back(aTile);
        }
    }
    //GRADE
    std::vector<Grades> result;
    for (int tile : goodTiles) {
        int o = getObjectFromArray(cache, tile);//id === tile
        int n = getColorUnderneath(o.id, size);
        /* *** the best pawn we do not want to move ***
         * larger pawn
         * 1 or 2 white pawns in a row
         * 1, 2, 3 black pawns in a row
         * a black pawn underneath
         * a white pawn underneath is not the best situation
         * */
        result.push_back(Grades(tile, ((4 - size) * 2) + o.countWhite + (o.countBlack * 2) + ((n === 0) ? 0 : (n === 1) ? -2 : 5)));
    }
    return result;
}

bool AI::findId(std::vector<int> numbers, int searchId) {
    return std::find(numbers.begin(), numbers.end(), searchId) != numbers.end();
}

int AI::getObjectFromArray(const std::vector<Grades>& objects, int searchId) {
    for (const auto& obj : objects) {
        if (obj.id == searchId) {
            return obj.grade;
        }
    }
    return -1;
}

int AI::getColorUnderneath(int tile, int size) {
    int number = pow(2, tile);
    for (int i = size + 1; (i < 4); i += 1) {
        if ((m_bState[1][i] & number) != 0) {
            return 1;
        }
        if ((m_bState[0][i] & number) != 0) {
            return 2;
        }
    }
    return 0;
}

std::vector<int> AI::getTilesOfSize(int size) {
    std::vector<int> wTiles;

    int number = m_visibleWhiteRows[size];
    int bitIndex = 0;

    while (number != 0) {
        if (number & 1) {  // Check if the least significant bit is 1
            wTiles.push_back(m_tiles[bitIndex]);  // Use the bitIndex to get the tile from m_tiles
        }
        number >>= 1;  // Shift the number to the right by 1 bit
        bitIndex++;    // Increment the bit position
    }

    return wTiles;
}

int AI::getPawnSize(int tile) {
    int mask = pow(2, tile);
    for (int i = 0; i < 4; i++) {
        if (((m_bState[0][i] | m_bState[0][i]) & mask) > 0) {
            return i;
        }
    }
    return 4;
}

std::vector<Grades> AI::sortWhiteRows() {
    std::vector<Grades> whiteBestRows;
    //var lowest = 21;
    for (int i = 0; i < 10; i++) {
        int j = m_randomMaskSequence[i];//shuffled numbers from 0 to 10
        int aMask = m_mask[j];
        int visibleWhiteOnRow = m_visibleWhite & aMask;
        //count the biggest black pawns and white pawns on the row if equal to four the row can't be attacked
        if (this->count1Bits(visibleWhiteOnRow ^ (aMask & m_bState[0][0])) != 4) {
            int countRow = (4 - count1Bits(visibleWhiteOnRow)) * 5; //number of tiles on row not occupied by white * 5
            countRow += (count1Bits(m_visibleWhiteRows[3] & aMask)) * 4;//tiles occupied by smallest white * 4
            countRow += (count1Bits(m_visibleWhiteRows[2] & aMask)) * 3;
            countRow += (count1Bits(m_visibleWhiteRows[1] & aMask)) * 2;
            countRow += (count1Bits(m_visibleWhiteRows[0] & aMask));
            countRow -= count1Bits((aMask & (m_visibleWhite | m_visibleBlack)) ^ aMask); //empty tiles

            /*//find lowest countRow with the smallest black pawn different from the biggest
            if ((countRow < lowest) && (getSmallestBlackPawnOfRow(aMask) !== 0)) {
                rowToAttack = j;
                lowest = countRow;
            }*/
            whiteBestRows.push_back(Grades(j, countRow));
        }
    }

    std::sort(whiteBestRows.begin(), whiteBestRows.end(), [](const Grades& a, const Grades& b) {
        return a.grade < b.grade;
    });

    return whiteBestRows;
}

void AI::chooseTileFromRowToAttack(int rowToAttack) {
    if (rowToAttack == -1) {
        //do random move
        m_toTile = startAttack(0);
    } else {
        int rowMask = m_mask[rowToAttack];
        int emptyTiles = (rowMask & (m_visibleWhite | m_visibleBlack)) ^ rowMask;
        if (emptyTiles == 0) {
            int s = getSmallestBlackPawnOfRow(rowMask);
            //emptyTiles = array of tiles with the smallest black pawns
            emptyTiles = m_bState[0][s];
            for (s -= 1; s > -1; s -= 1) {
                emptyTiles = (emptyTiles ^ (m_bState[0][s] | m_bState[1][s])) & emptyTiles;
            }
        }
        m_toTile = getNextMove(rowMask, emptyTiles, false);
    }
}

/**
 * Find rows with the maxCount of gobblers
 */
std::vector<int> AI::rowCheck(int maxCount, bool isWhite, bool removeRowsWithSize0) {
    std::vector<int> rows;
    int number = (isWhite ? m_visibleWhite : m_visibleBlack);
    for (int i = 0; i < 10; i++) {
        if (this->count1Bits(m_mask[i] & number) == maxCount) {
            if (removeRowsWithSize0) {
                if ((m_bState[!isWhite][0] & m_mask[i]) == 0) {
                    rows.push_back(i);
                }
            } else {
                rows.push_back(i);
            }
        }
    }
    return rows;
}

int AI::getSmallestBlackPawnOfRow(int mask) {
    int s = 4;
    for (int i = 3; i > -1; i -= 1) {
        if ((mask & m_visibleBlackRows[i]) != 0) {
            s = i;
            break;
        }
    }
    return s;
}

int AI::count1Bits(int x) {
    int count = 0;
    while (x) {
        x &= x - 1;
        count++;
    }
    return count;
};

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
