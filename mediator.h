#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QtQuick/QQuickItem>
#include <QVector>
#include <QObject>
#include <QQuickItem>
#include "gobbler.h"
#include "board.h"

class Mediator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<Gobbler*> whiteList READ getWhiteList NOTIFY whiteListChanged)
    Q_PROPERTY(QList<Gobbler*> blackList READ getBlackList NOTIFY blackListChanged)
    Q_PROPERTY(bool blackTurn READ isBlackTurn NOTIFY blackTurnChanged)
    Q_PROPERTY(Gobbler* selection READ getSelection NOTIFY selectionChanged)
    Q_PROPERTY(Board* board READ getBoard CONSTANT)
    QML_ELEMENT

public:
    Mediator(QQuickItem *parent = 0);
    QList<Gobbler*> getWhiteList() const;
    QList<Gobbler*> getBlackList() const;
    bool isBlackTurn() const;
    Gobbler* getSelection() const;
    Board* getBoard() const;

public slots:
    void addWhiteItem(Gobbler *gobbler);
    void addBlackItem(Gobbler *gobbler);
    void toggleBlackTurn();
    void setSelection(Gobbler* gobbler);
    void setBoard(Board* board);

    void repaint();
    void onClick(Matrix *matrix, const double x, const double y);

signals:
    void whiteListChanged();
    void blackListChanged();
    void blackTurnChanged();
    void selectionChanged();

private:
    QList<Gobbler*> m_whiteList;
    QList<Gobbler*> m_blackList;
    bool m_blackTurn;
    Gobbler* m_selection;
    Board* m_board;
};

#endif // MEDIATOR_H
