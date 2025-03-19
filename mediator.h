#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QtQuick/QQuickItem>
#include <QVector>
#include <QObject>
#include "gobbler.h"
#include "board.h"
#include "bridge.h"

class Mediator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Gobbler*> list READ getList NOTIFY listChanged)
    Q_PROPERTY(bool blackTurn READ isBlackTurn NOTIFY blackTurnChanged)
    Q_PROPERTY(Gobbler* selection READ getSelection NOTIFY selectionChanged)
    Q_PROPERTY(Board* board READ getBoard CONSTANT)
    Q_PROPERTY(Bridge* comm MEMBER m_comm)
    QML_ELEMENT

public:
    Mediator(QObject *parent = 0);
    QList<Gobbler*> getList() const;
    bool isBlackTurn() const;
    Gobbler* getSelection() const;
    Board* getBoard() const;
    Bridge* m_comm;
    QTimer *timer;

public slots:
    void addItem(Gobbler *gobbler);
    void toggleBlackTurn();
    void setSelectionByTile(int tile);
    void setSelection(int roundX, int borderZ);
    void setSelection(Gobbler* gobbler);
    void setBoard(Board* board);

    void repaint();
    void onClick(Matrix *matrix, const double x, const double y);

private slots:
    bool checkWinner(bool player);
    int getTileFromCoord(int x, int z);
    void updateDepthOfGobblers(int x, int z);
    void updateState(int x, int y, int z, int oldTile, int newTile, Matrix *matrix);
    void updateGobbler();
    void afterAnimation();
    void startAi(bool aiTurn);
    void writeLog();
    void tests();

signals:
    void listChanged();
    void blackTurnChanged();
    void selectionChanged();

private:
    QList<Gobbler*> m_list;
    bool m_blackTurn = false;
    Gobbler* m_selection = nullptr;
    Board* m_board = nullptr;
    bool m_lock;

    Matrix *matrx;
    int newX;
    int newY;
    int newZ;

    int boardX;
    int boardY;
    int boardZ;

    int myNewTile;

    int m_state [2][4] = {{ 0, 0, 0, 0}, {0, 0, 0, 0}};
    int m_mask [10] = {61440, 3840, 240, 15, 34952, 17476, 8738, 4369, 33825, 4680};
    int m_speed = 50;

};

#endif // MEDIATOR_H
