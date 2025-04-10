#include <QtTest/QtTest>
#include "ai.h"

class TestAI : public QObject {
    Q_OBJECT

private slots:
    void winTakeFromStack();
    void winWhiteBlocksWithSize3();
    void winWhiteBlocksWithSize0();
    void winWhiteBlocksWithSize1AllSize0OnBoard();
    void winWhiteBlocksWithSize2AllOnBoard();
    void dontLoseTakeFromStack();
    void dontLoseTakeFromBoard();
    void dontLoseBlockCross();
    void attackCenter();
    void attackRowWith3Gobblers();
    void attackRowWith3GobblersWithSmallDefence();
    void attackRowWith3GobblersWithSmallDefence2();
    void attackBug1();
    void attackBug2();
};

/* TRY TO WIN */
void TestAI::winTakeFromStack() {
    AI computer(1);
    /* 1000 0000 0000 0000  0000 0000 0000 0000         15 14 13 12
     * 0000 1000 0000 0000  0000 0000 0000 0000         11 10 09 08
     * 0000 0000 1000 0000  0000 0000 0000 0000         07 06 05 04
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00 */
    int state [2][4] = {{32768, 2048, 128, 0}, {0, 0, 0, 0}};
    aiMove move = computer.move(state);
    QCOMPARE(move.from(), 19); // smallest pawn stack
    QCOMPARE(move.to(), 3);
}

void TestAI::winWhiteBlocksWithSize3() {
    AI computer(1);
    int state2 [2][4] = {{32768, 2048, 128, 0}, {0, 0, 0, 8}};
    aiMove move = computer.move(state2);
    QCOMPARE(move.from(), 16); // biggest pawn stack
    QCOMPARE(move.to(), 3);
}

void TestAI::winWhiteBlocksWithSize0() {
    AI computer(1);
    int state3 [2][4] = {{32768, 2048, 128, 0}, {8, 0, 0, 0}};
    aiMove move = computer.move(state3);
    QCOMPARE(move.from(), 19);
    QCOMPARE(move.to() == 3, false);
}

void TestAI::winWhiteBlocksWithSize1AllSize0OnBoard() {
    AI computer(1);
    int state4 [2][4] = {{49184, 51200, 49280, 0}, {0, 8, 0, 0}};
    aiMove move = computer.move(state4);
    QCOMPARE(move.from() == 14 || move.from() == 5, true);
    QCOMPARE(move.to(), 3);
}

void TestAI::winWhiteBlocksWithSize2AllOnBoard() {
    AI computer(1);
    /* 1100 0000 0010 0000  0010 0000 0000 0010         15 14 13 12
     * 1100 1000 0000 0000  0000 0000 0010 0000         11 10 09 08
     * 1100 0000 1000 0000  0000 0000 0000 1000         07 06 05 04
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00 */
    int state5 [2][4] = {{49184, 51200, 49280, 0}, {8706, 32, 8, 0}};
    aiMove move = computer.move(state5);
    QCOMPARE(move.from() == 15 || move.from() == 11 || move.from() == 14, true);
    QCOMPARE(move.to(), 3);
}

/* DONT LOSE */
void TestAI::dontLoseTakeFromStack() {
    AI computer(1);
    /* 0000 0110 0000 0000  0111 0000 0000 0000         15 14 13 12
     * 0000 0000 0000 0000  0000 1000 0000 0000         11 10 09 08
     * 0000 0000 0000 0000  0000 0000 1000 0000         07 06 05 04
     * 0000 0000 0000 0000  0000 0000 0000 1000         03 02 01 00 */
    int state6 [2][4] = {{1536, 0, 0, 0}, { 28672, 2048, 128, 8}};
    aiMove move = computer.move(state6);
    QCOMPARE(move.from(), 16);
    QCOMPARE(move.to(), 15);
}

void TestAI::dontLoseTakeFromBoard() {
    AI computer(1);
    /* 0 2 2 2
     * 2 1 1 0
     * 2 1 0 0
     * 2 0 0 0 */
    int state7 [2][4] = {{1600, 0, 0, 0}, { 28672, 2048, 128, 8}};
    aiMove move = computer.move(state7);
    QCOMPARE(move.from() == 6 || move.from() == 9 || move.from() == 10, true);
    QCOMPARE(move.to(), 15);
}

void TestAI::dontLoseBlockCross() {
    AI computer(2);
    /* Block if 1 sized is placed on 9 no possibility to block */
    /* 0000 0000 0000 0000  0000 1001 0000 0000         15 14 13 12    0 0 1 0
     * 0000 0000 0000 0000  0010 0000 0000 0010         11 10 09 08    1 0 0 1
     * 0000 0000 0000 0000  0000 0000 0000 1000         07 06 05 04    0 0 0 0
     * 0000 0000 0000 0000  0000 0000 0000 0000         03 02 01 00    1 0 1 0 */
    int state8 [2][4] = {{0, 0, 0, 0}, { 2304, 8194, 0, 0}};
    aiMove move = computer.move(state8);
    QCOMPARE(move.from(), 16);
    QCOMPARE(move.to(), 9);
}

/* Attack */
void TestAI::attackCenter() {
    AI computer(2);
    // put on the middle
    int state9 [2][4] = {{1, 2, 0, 0}, { 12, 32, 0, 0}};
    aiMove move = computer.move(state9);
    QCOMPARE(move.from(), 18);
    QCOMPARE(move.to() == 6 || move.to() == 9 || move.to() == 12 || move.to() == 15, true);
}

void TestAI::attackRowWith3Gobblers() {
    AI computer(1);
    int state10 [2][4] = {{128, 1, 0, 0}, { 8, 4, 2, 0}};
    aiMove move = computer.move(state10);
    QCOMPARE(move.from(), 16);
    QCOMPARE(move.to(), 1);
}

void TestAI::attackRowWith3GobblersWithSmallDefence() {
    AI computer(1);
    /* 1 0 0 2
     * 0 2 1 1
     * 0 1 1 2
     * 2 1 0 2 */
    int state11 [2][4] = {{5136, 1, 8, 4}, { 32864, 772, 2, 1024}};
    aiMove move = computer.move(state11);
    QCOMPARE(move.from(), 10);
    QCOMPARE(move.to(), 8);
}

void TestAI::attackRowWith3GobblersWithSmallDefence2() {
    AI computer(2);
    /* 1 0 0 2         15 14 13 12
     * 2 2 1 0         11 10 09 08
     * 1 0 2 1         07 06 05 04
     * 2 0 1 2         03 02 01 00 */
    int state12 [2][4] = {{7168, 40, 1, 32}, {32898, 2560, 1056, 16}};
    aiMove move = computer.move(state12);
    QCOMPARE(move.from(), 12);
    QCOMPARE(move.to(), 5);
}

void TestAI::attackBug1() {
    AI computer(2);
    /* 2 2 0 1         15 14 13 12
     * 0 1 2 1         11 10 09 08
     * 2 1 1 2         07 06 05 04
     * 1 0 0 2         03 02 01 00 */
    int state13 [2][4] = {{1064, 36928, 17, 128}, {33281, 16528, 32768, 4096}};
    aiMove move = computer.move(state13);
    QCOMPARE(move.from(), 3);
    QCOMPARE(move.to(), 12);
}

void TestAI::attackBug2() {
    AI computer(2);
    /* 1 1 2 2         15 14 13 12
     * 0 1 1 2         11 10 09 08
     * 2 2 2 1         07 06 05 04
     * 2 0 0 1         03 02 01 00 */
    int state14 [2][4] = {{1552, 53248, 128, 1}, {4168, 768, 544, 16512}};
    aiMove move = computer.move(state14);
    QCOMPARE(move.from(), 14);
    QCOMPARE(move.to(), 5);
}

// Main function to run the test
QTEST_MAIN(TestAI)
#include "test_ai.moc"
