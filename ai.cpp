#include "ai.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <bitset>
#include <chrono>

AI::AI(int level) {
    m_level = level;
    std::array<int, 10> sequence { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sequence.begin(), sequence.end(), std::default_random_engine(seed));

    for (int i = 0; i < 10; i++) {
        m_randomMaskSequence[i] = sequence[i];
    }
}

// Move function
aiMove AI::move(int boardState [2][4]) {

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

    if (m_fromTile == -1 || m_toTile == -1) { // no pawn or tile found
        //writeLog(bState);
        randomMove();
        //std::cout << "current tile: " << nextMove << std::endl;
    }

    aiMove move;
    move.setFrom(m_fromTile);
    move.setTo(m_toTile);
    return move;

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
    std::vector<int> rows = rowCheck(3, false, false);
    for (int row : rows) {
        int maskToCheck = m_mask[row];
        m_toTile = getToTile(maskToCheck, m_visibleBlack, true);
        int size = ((int)pow(2, m_toTile) & m_visibleWhite) != 0 ? getPawnSize(m_toTile) : 4;
        if (size > 0) {
            return !setFromTile(size, {row});
        }
    }
    return true;
}

// Dont lose
bool AI::dontLose() {
    std::vector<int> rows = rowCheck(3, true, true);
    if (rows.size() > 1) {
        int maskCrossing = m_mask[rows[0]];
        for (int row : rows) {
            maskCrossing &= m_mask[row]; // row 0 is taken twice; but it's oke
        }
        if (maskCrossing > 0) {
            m_toTile = getToTile(0, maskCrossing, false);
            if (getPawnSize(m_toTile) != 0) {
                if (setFromTile(1, rowCheck(3, true, false))) { // if 1 only a 0 size gobbler can go gobble it; exclude rows with 3 whites
                    return false;
                }
            }
        }
    } else if (rows.size() == 1) {
        int maskToCheck = m_mask[rows[0]];
        int s = getSmallestWhiteOfRow(m_mask[rows[0]]);
        if (s == 0) { // all 3 big pawns
            m_toTile = getToTile(maskToCheck, m_visibleWhite, true); // get first empty tile
            if (setFromTile(1, rows)) {
                return false;
            }
        }
        m_toTile = getToTile(maskToCheck, m_visibleWhiteRows[s], false);
        if (setFromTile(s, rows)) {
            return false;
        }
    }
    return true;
}

bool AI::block() {
    // if level is 1: check if white player can put a size0 on a tile creating 2 row with 3 pawns.
    // (no biggest white pawns on the rows)
    // fake a white pawn size1 on that tile and do don't loose
    if (m_level == 2) {
        bool ret = true;

        int i = 0;
        for (int twiceThree : m_twiceThree) {
            if ((count1Bits(m_visibleWhite & twiceThree) - (count1Bits(m_bState[0][0] & twiceThree)) > 3)
                && ((m_bState[1][0] & m_blockMove[i]) == 0)) { // no white on the block move

                int oldValue = m_bState[1][1];
                int old2 = m_visibleWhite;
                int old3 = m_visibleWhiteRows[1];

                m_bState[1][1] |= m_blockMove[i];
                m_visibleWhite |= m_blockMove[i];
                m_visibleWhiteRows[1] |= m_blockMove[i];

                ret = dontLose();

                m_bState[1][1] = oldValue;
                m_visibleWhite = old2;
                m_visibleWhiteRows[1] = old3;
                break;
            }
            i++;
        }
        return ret;
    }
    return true;
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
            if (setFromTile(s, getRowsOfTile(m_toTile))) {
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
    int random = std::rand();
    int tile = 0;
    for (int i = 0; i < 16 && s == 0; i++) {
        tile = random++ % 16;
        if (m_level < 2 || ((tile & m_crossings) > 0 && (getColorUnderneath(tile, -1) < 2))) {
            s = getPawnSize(tile);
        }
    }
    while (s == 0) {
        tile = random++ % 16;
        s = getPawnSize(tile);
    }
    m_toTile = tile;

    int stackSize = getPawnFromStack(s - 1);
    if (stackSize > -1) {
        m_fromTile = 16 + stackSize;
    }
    int toTile = 1 << m_toTile;
    if (m_fromTile == -1) {
        for (int i = s - 1; i > 0; i--) {
            int pawnsOnBoard = m_visibleBlackRows[i];
            int possibleTiles = pawnsOnBoard & ~(toTile);
            if (possibleTiles > 0) {
                m_fromTile = get_first_set_bit_position(possibleTiles);
                break;
            }
        }
    }
}

void AI::attackFallBack() {
    int ignore = 0;
    for (int i = 0; i < 4; i += 1) {
        int size = getPawnSize(m_toTile);
        size = (size == -1 ? 4 : size);
        if (!setFromTile(size, getRowsOfTile(m_toTile))) {
            ignore += pow(2, m_toTile);
            m_toTile = startAttack(ignore);
        } else {
            break;
        }
    }
}

int AI::startAttack(int ignore) {
    int num = m_crossings ^ m_bState[0][0] ^ m_bState[1][0] ^ ignore;
    return 1 << get_first_set_bit_position(num); // pow 2 of position of 1 bit
}

int AI::getToTile(int rowMask, int visiblePawns, bool reverse) {
    int result = (rowMask == 0) ? visiblePawns : (rowMask & visiblePawns);
    if (rowMask != 0 && reverse) {
        result ^= rowMask;
    }
    return get_first_set_bit_position(result);
}

bool AI::setFromTile(int size, std::vector<int> rows) {
    bool ret = false;
    int solution = getPawnFromStack(size);
    if (solution > -1) {
        m_fromTile = 16 + solution;
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
        int onBoard = count1Bits(m_bState[0][i]);
        if (onBoard < maxNo) {
            pawnSize = i;
        }
        maxNo = onBoard;
    }
    return pawnSize;
}

int AI::getPawnFromBoard(int size, std::vector<int> excludeRows) {
    if (m_level < 2) {
        // get pawn from board which pawnSize < size
        std::vector<int> whiteWins = rowCheck(3, true, false);
        for (int i = size - 1; i >= 0; i--) {
            int pawns = m_visibleBlackRows[i];

            // exclude: white wins rows
            for (int ww : whiteWins) {
                pawns &= ~m_mask[ww]; // NAND & and not
            }
            // exclude: exclude rows
            for (int exc : excludeRows) {
                pawns &= ~m_mask[exc]; // NAND & and not
            }
            // return: tile
            return get_first_set_bit_position(pawns);
        }
    } else {
        int result = -1;
        for (int i = size - 1; i > -1; --i) {
            std::vector<Grades> graded = getUnimportantBlackTile(i, excludeRows);

            auto maxId = std::max_element(graded.begin(), graded.end(),
                   [](const Grades& a, const Grades& b) {
                       return a.grade < b.grade;
                   });
            if (maxId != graded.end() && maxId->id > result) {
                result = maxId->id;
            }
        }

        if (result > 0) {
            return result;
        }

        for (int i = size - 1; i >= 0; i--) {
            int pawns = m_visibleBlackRows[i];

            if (pawns > 0) {
                return pow(2, std::bitset<16>(pawns).count() - 1);
            }
        }
    }
    return -1;
}

std::vector<Grades> AI::getUnimportantBlackTile(int size, std::vector<int> excludeRows) {
    std::vector<int> bTiles = getTilesOfSize(size); //get the tile of a visible black pawns from a certain size
    std::vector<int> goodTiles;

    std::vector<tileCounter> cache;
    for (int aTile : bTiles) {
        bool bad = false;
        //tileRows contains the row indexes, these indexes are the same as the mask indexes for those rows
        std::vector<int> tileRows = getRowsOfTile(aTile);
        //loop over every row of the tile (max 3)
        //a white pawn is only unimportant if it's not blocking a row or not of strategic interest for a row
        for (int r : tileRows) {
            int p = pow(2, aTile);
            //stop processing the tile if it's in a row with 3 black whites, or the row that is currently formed.
            if (findId(rowCheck(3, true, false), r) || findId(excludeRows, r)) {
                bad = true;
                break;
            } else if ((count1Bits(m_mask[r] & m_visibleWhite) == 2) &&
                       ((((m_mask[r] & m_bState[0][0]) | p) ^ p) == 0) &&
                       (getColorUnderneath(aTile, size) == 1)) {
                //Stop when all conditions are true
                // - 2 white pawns
                // - no biggest black pawn (except the one being checked)
                // - and one black pawn underneath (the one being checked)
                bad = true;
                break;
            } else {
                //Set cache for Grade
                //Count for every row the tile is in, the white and black pawns.
                tileCounter* o = getObjectFromArray(cache, aTile);//id === tile
                if (o != nullptr) {
                    o -> addWhiteCounter(count1Bits((m_visibleWhite & m_mask[r])));
                    o -> addBlackCounter(count1Bits((m_visibleBlack & m_mask[r])));
                } else {
                    tileCounter tc(aTile, count1Bits((m_visibleWhite & m_mask[r])), count1Bits((m_visibleBlack & m_mask[r])));
                    cache.push_back(tc);
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
        tileCounter* o = getObjectFromArray(cache, tile);
        if (o == nullptr) continue;
        int n = getColorUnderneath(tile, size);
        /* *** the best pawn we do not want to move ***
         * larger pawn
         * 1 or 2 white pawns in a row
         * 1, 2, 3 black pawns in a row
         * a black pawn underneath
         * a white pawn underneath is not the best situation
         * */
        result.push_back(
            Grades(tile,
                   ((4 - size) * 2) + o->whiteCounter() + (o->blackCounter() * 2) + ((n == 0) ? 0 : (n == 1) ? -2 : 5)));
    }
    return result;
}

bool AI::findId(std::vector<int> numbers, int searchId) {
    return std::find(numbers.begin(), numbers.end(), searchId) != numbers.end();
}

tileCounter* AI::getObjectFromArray(std::vector<tileCounter>& counters, int tile) {
    for (tileCounter& obj : counters) {
        if (obj.tile() == tile) {
            return &obj;
        }
    }
    return nullptr;
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
    std::vector<int> bTiles;

    int number = m_visibleBlackRows[size];
    int bitIndex = 0;

    while (number != 0) {
        if (number & 1) {  // Check if the least significant bit is 1
            bTiles.push_back(bitIndex);
        }
        number >>= 1;  // Shift the number to the right by 1 bit
        bitIndex++;    // Increment the bit position
    }

    return bTiles;
}

int AI::getPawnSize(int tile) {
    int mask = pow(2, tile);
    for (int i = 0; i < 4; i++) {
        if (((m_bState[0][i] | m_bState[1][i]) & mask) > 0) {
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
        int cTiles = (rowMask & (m_visibleWhite | m_visibleBlack)) ^ rowMask;
        if (cTiles == 0) {
            int s = getSmallestWhiteOfRow(rowMask);
            //cTiles = array of tiles with the smallest black pawns
            cTiles = m_bState[0][s];
            for (s -= 1; s > -1; s -= 1) {
                cTiles = (cTiles ^ (m_bState[0][s] | m_bState[1][s])) & cTiles;
            }
        }
        if (m_level == 2 && ((cTiles & m_crossings) > 0)) {
            cTiles &= m_crossings;
        }
        m_toTile = getToTile(rowMask, cTiles, false);
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

int AI::getSmallestWhiteOfRow(int mask) {
    for (int i = 3; i > -1; i--) {
        if ((mask & m_visibleWhiteRows[i]) != 0) {
            return i;
        }
    }
    return 4;
}

int AI::count1Bits(int x) {
    //return std::bitset<16>(x).count();
    int count = 0;
    while (x) {
        x &= x - 1;
        count++;
    }
    return count;
}

int AI::get_first_set_bit_position(int num) {
    int bitCount = std::bitset<16>(num).count(); // count number of 1 bits

    if (bitCount == 0) {
        return -1; // No 1 bits, return 0
    }

    // Generate a random number between 1 and bitCount
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, bitCount);
    int randomBitIndex = dist(gen);

    // Locate the chosen 1 bit
    int currentBitIndex = 0;
    for (int i = 0; i < 16; ++i) {
        if (num & (1 << i)) { // Check if the i-th bit is 1
            ++currentBitIndex;
            if (currentBitIndex == randomBitIndex) {
                return i;
            }
        }
    }

    return -1; // Should never reach here

    /*if (n == 0) return -1;
    return log2(n & -n);*/
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
