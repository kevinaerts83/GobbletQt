#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QtQuick/QQuickItem>
#include <QVector>
#include <QObject>
#include "gobbler.h"
#include "board.h"
#include "state.h"

class Mediator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Gobbler*> list READ getList NOTIFY listChanged)
    Q_PROPERTY(bool blackTurn READ isBlackTurn NOTIFY blackTurnChanged)
    Q_PROPERTY(Gobbler* selection READ getSelection NOTIFY selectionChanged)
    Q_PROPERTY(Board* board READ getBoard CONSTANT)
    Q_PROPERTY(State* state MEMBER m_comm)
    QML_ELEMENT

public:
    Mediator(QObject *parent = 0);
    QList<Gobbler*> getList() const;
    bool isBlackTurn() const;
    Gobbler* getSelection() const;
    Board* getBoard() const;
    State* m_comm;
    QTimer *timer;

public slots:
    void addItem(Gobbler *gobbler);
    void toggleBlackTurn();
    void setSelectionByTile(int tile);
    void setSelection(int roundX, int borderZ);
    void setSelection(Gobbler* gobbler);
    void setBoard(Board* board);
    void setMatrix(Matrix* matrix);
    void startAi(bool aiTurn);
    void resetItems(double width, double height);

    void repaint();
    bool onClick(const double x, const double y);

private slots:
    bool checkWinner(bool player);
    int getTileFromCoord(int x, int z);
    void updateDepthOfGobblers(int x, int z);
    void updateState(int x, int y, int z, int oldTile, int newTile);
    void updateGobbler();
    void afterAnimation();
    bool isStack(double x);
    bool isNextToBoard(double x, double z, bool margin);
    int getTileX(int tile);
    int getTileY(int tile);
    int roundCoord(double coord);
    int getBorderZ(int x, double y, int z);
    bool isValidMove(int oldTile, int newTile);

    void writeLog();

signals:
    void listChanged();
    void blackTurnChanged();
    void selectionChanged();

private:
    QList<Gobbler*> m_list;
    bool m_blackTurn = false;
    Gobbler* m_selection = nullptr;
    Board* m_board = nullptr;
    Matrix* m_matrix = nullptr;
    bool m_lock;

    int newX;
    int newY;
    int newZ;

    int boardX;
    int boardY;
    int boardZ;

    int myNewTile;

    int m_state [2][4] = {{ 0, 0, 0, 0}, {0, 0, 0, 0}};

    constexpr static const int WINNING_NO [10] = {61440, 3840, 240, 15, 34952, 17476, 8738, 4369, 33825, 4680};
    static const int SPEED = 50;
    static const int MAX_TILE_INDEX = 15;
    static const int TIMER_VALUE = 20;
};

#endif // MEDIATOR_H
