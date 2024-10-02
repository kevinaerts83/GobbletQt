#include "AI.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <bitset>

// std::size_t

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
    std::vector<int> rows = rowCheck(false, 3);
    if (rows.size() > 1) {
        int maskCrossing = mask[rows[0]];
        for (int i = 1; i < rows.size(); ++i) {
            maskCrossing &= mask[rows[i]];
        }
        if (maskCrossing > 0) {
            m_toTile = getNextMove(0, maskCrossing, false);
            if (getPawnSize(m_toTile, pawns) != 0) {
                if (setNextPawn(1, rows)) {
                    return false;
                }
            }
        }
    } else if (rows.size() == 1) {
        int maskToCheck = mask[rows[0]];
        int s = getSmallestBlackPawnOfRow(mask[rows[0]]);
        m_toTile = getNextMove(maskToCheck, visibleBlackRows[s], false);
        if (setNextPawn(s, rows)) {
            return false;
        }
        m_toTile = getNextMove(maskToCheck, visibleBlack, true);
        if (setNextPawn(1, rows)) {
            return false;
        }
    }
    return true;
}

bool AI::block() {
    //if level is 1: check if black player can put a size0 on a tile creating 2 row with 3 pawns. (no biggest white pawns on the rows)
    //fake a black pawn size1 on that tile and do don't loose
    if (level === 1) {
        var i, l, oldValue, old2, old3, ret = true,
            masks = [63624, 62532, 61986, 61713, 36744, 20292, 12066, 7953, 35064, 17652, 8946, 4593, 34959, 17487, 8751, 4383, 62497, 36641, 34033, 33839, 62024, 8008, 4856, 4687, 36009, 50277, 42531, 38193, 39624, 22092, 12906, 4953],
            crossing = [32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1, 32768, 1024, 32, 1, 4096, 512, 64, 8, 32768, 1024, 32, 1, 8, 64, 512, 4096];
        for (i = 0, l = masks.length; i < l; i += 1) {
            if ((count1Bits(visibleBlack & masks[i]) > 3) && ((visibleBlack & crossing[i]) === 0) && ((bState[1][0] & masks[i]) === 0)) {
                oldValue = m_bState[0][1];
                old2 = visibleBlack;
                old3 = visibleBlackRows[1];

                bState[0][1] |= crossing[i];
                visibleBlack |= crossing[i];
                visibleBlackRows[1] |= crossing[i];

                ret = dontLose(bState, pawns);

                bState[0][1] = oldValue;
                visibleBlack = old2;
                visibleBlackRows[1] = old3;
                break;
            }
        }
        return ret;
    }
    return false;
}

void AI::attack() {
    var s = 0, i, rowToAttack, wRows = [];

    if (m_visibleWhite !== 0) {
        //determine whites best row lower is better
        wRows = sortWhiteRows(bState);
    }
    for (i = 0; i < 10; i += 1) {
        rowToAttack = (wRows.length > i)  ? (wRows[i].rowNumber) : undefined;
        //set the nextMove
        chooseTileFromRowToAttack(bState, rowToAttack);

        if (m_toTile >= 0) {
            s = getPawnSize(m_toTile, pawns);
            s = (s === -1 ? 4 : s);
            if (setNextPawn(s, getRowsOfTile(m_toTile))) {
                break;
            }
        }
        if (wRows.length < i) {
            break;
        }
    }

    if (m_toTile < 0) {
        attackFallBack(bState, pawns);
    }
}

void AI::randomMove() {
    int s = 0;
    while (s == 0) {
        int t = std::rand() % 16;
        m_toTile = tiles[t];
        s = getPawnSize(m_toTile, pawns);
    }
    for (int i = pawns.size() - 1; i >= 0; --i) {
        p = pawns[i];
        if ((p.isTurn === 1) && p.visible && (p.tile !== m_toTile) && (p.size < s || s === 4)) {
            this->pawn = pawns[i];
            break;
        }
    }
}

void AI::attackFallBack() {
    var i, size, ignore = 0;
    for (i = 0; i < 4; i += 1) {
        size = getPawnSize(m_toTile, pawns);
        size = (size === -1 ? 4 : size);
        if (!setNextPawn(size, getRowsOfTile(m_toTile))) {
            m_toTile = startAttack(bState, ignore);
            ignore += Math.pow(2, tiles.indexOf(m_toTile));
        } else {
            break;
        }
    }
}

void AI::startAttack() {
    var c = (crossings ^ bState[0][0] ^ bState[1][0] ^ ignore).toString(2), rnd = c.length, t;
    rnd = Math.floor(Math.random() * rnd);
    t = c.indexOf('1', rnd);
    while (t === -1) {
        rnd = Math.floor(Math.random() * rnd);
        t = c.indexOf('1', rnd);
    }
    return tiles[15 - t];//15=tiles.length - 1
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
            std::vector<std::vector<int>> graded = getUnimportantWhiteTile(i, excludeRows);
            for (std::vector<int> grade : graded) {
                if ((j == 0) || (j > grade[1])) {
                    j = grade[1];
                    result = grade[0];
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

std::vector<std::vector<int>> AI::getUnimportantWhiteTile(int size, std::vector<int> excludeRows) {
    var i, j, l, lj, o, n, r, p, bad, aTile, cache = [], result = [], tileRows, wTiles = getTilesOfSize(size); //get the tile of a visible white pawns from a certain size

    for (i = 0, l = wTiles.length; i < l; i += 1) {
        bad = false;
        aTile = wTiles[i];
        //tileRows contains the row indexes, these indexes are the same as the mask indexes for those rows
        tileRows = getRowsOfTile(aTile);
        //loop over every row of the tile (max 3)
        //a white pawn is only unimportant if it's not blocking a row or not of strategic interest for a row
        for (j = 0, lj = tileRows.length; j < lj; j += 1) {
            r = tileRows[j];
            p = Math.pow(2, tiles.indexOf(aTile));
            //stop processing the tile if it's in a row with 3 black pawns, or the row that is currently formed.
            if ((rowCheck(false, 3).indexOf(r) !== -1) || (excludeRows.indexOf(r) !== -1)) {
                bad = true;
                break;
            } else if ((count1Bits(mask[r] & visibleBlack) === 2) &&
                       ((((mask[r] & bState[1][0]) | p) ^ p) === 0) &&
                       (getColorUnderneath(aTile, size, bState) === 2)) {
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
                    o.countWhite += count1Bits((visibleWhite & mask[r]));
                    o.countBlack += count1Bits((visibleBlack & mask[r]));
                } else {
                    cache.push({id: aTile,
                                countWhite: count1Bits((visibleWhite & mask[r])),
                                countBlack: count1Bits((visibleBlack & mask[r]))});
                }
            }
        }
        if (bad === false) {
            //initialize result array for tiles that passed the test.
            result.push({id: aTile, grade: 0});
        }
    }
    //GRADE
    for (i = 0; i < result.length; i += 1) {
        o = getObjectFromArray(cache, result[i].id);//id === tile
        n = getColorUnderneath(o.id, size, bState);
        /* *** the best pawn we do not want to move ***
         * larger pawn
         * 1 or 2 white pawns in a row
         * 1, 2, 3 black pawns in a row
         * a black pawn underneath
         * a white pawn underneath is not the best situation
         * */
        result[i].grade = ((4 - size) * 2) + o.countWhite + (o.countBlack * 2) + ((n === 0) ? 0 : (n === 1) ? -2 : 5);
    }
    return result;
}

int AI::getObjectFromArray(int objects [], int value) {
    for (int o = 0, l = objects.length; o < l; o++) {
        if (objects[o].id === value) {
            return objects[o];
            break;
        }
    }
    return -1;
}

void AI::getColorUnderneath() {
    var i, number = Math.pow(2, tiles.indexOf(tile)), ret = 0;
    for (i = size + 1; (i < 4) && (ret === 0); i += 1) {
        if ((bState[1][i] & number) !== 0) {
            ret = 1;
        }
        if ((bState[0][i] & number) !== 0) {
            ret = 2;
        }
    }
    return ret;
}

void AI::getTilesOfSize() {
    if (size <= 3) {
        var wTiles = [],
            b = visibleWhiteRows[size].toString(2),
            l = b.length - 1,
            start = b.indexOf('1');

        while (start > -1) {
            wTiles.push(tiles[l - start]);
            start = b.indexOf('1', start + 1);
        }

        return wTiles;
    }
    return [];
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

void AI::sortWhiteRows() {
    var i, j, visibleWhiteOnRow, aMask, countRow, whiteBestRows = [];
    //var lowest = 21;
    for (i = 0; i < 10; i += 1) {
        j = randomMaskSequence[i];//shuffled numbers from 0 to 10
        aMask = mask[j];
        visibleWhiteOnRow = visibleWhite & aMask;
        //count the biggest black pawns and white pawns on the row if equal to four the row can't be attacked
        if (this->count1Bits(visibleWhiteOnRow ^ (aMask & bState[0][0])) !== 4) {
            countRow = (4 - count1Bits(visibleWhiteOnRow)) * 5; //number of tiles on row not occupied by white * 5
            countRow += (count1Bits(visibleWhiteRows[3] & aMask)) * 4;//tiles occupied by smallest white * 4
            countRow += (count1Bits(visibleWhiteRows[2] & aMask)) * 3;
            countRow += (count1Bits(visibleWhiteRows[1] & aMask)) * 2;
            countRow += (count1Bits(visibleWhiteRows[0] & aMask));
            countRow -= count1Bits((aMask & (visibleWhite | visibleBlack)) ^ aMask); //empty tiles

            /*//find lowest countRow with the smallest black pawn different from the biggest
            if ((countRow < lowest) && (getSmallestBlackPawnOfRow(aMask) !== 0)) {
                rowToAttack = j;
                lowest = countRow;
            }*/
            whiteBestRows.push({rowCount: countRow, rowNumber: j});
        }
    }
    whiteBestRows.sort(function (a, b) { return a.rowCount - b.rowCount; });
    return whiteBestRows;
}

void AI::chooseTileFromRowToAttack() {
    var rowMask, emptyTiles, s;
    if (rowToAttack === undefined) {
        //do random move
        m_toTile = startAttack(bState, 0);
    } else {
        rowMask = mask[rowToAttack];
        emptyTiles = (rowMask & (visibleWhite | visibleBlack)) ^ rowMask;
        if (emptyTiles === 0) {
            s = getSmallestBlackPawnOfRow(rowMask);
            //emptyTiles = array of tiles with the smallest black pawns
            emptyTiles = bState[0][s];
            for (s -= 1; s > -1; s -= 1) {
                emptyTiles = (emptyTiles ^ (bState[0][s] | bState[1][s])) & emptyTiles;
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

void AI::getSmallestBlackPawnOfRow() {
    var i, s = 4;
    for (i = 3; i > -1; i -= 1) {
        if ((mask & visibleBlackRows[i]) !== 0) {
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
