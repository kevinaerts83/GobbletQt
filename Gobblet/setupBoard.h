#ifndef SETUPBOARD_H
#define SETUPBOARD_H

#include <QObject>

#include "state.h"
#include "mediator.h"
#include "gobbler.h"

class SetupBoard : public QObject {
    Q_OBJECT
public:
    explicit SetupBoard(Mediator *mediator, QObject *parent = nullptr)
        : QObject(parent), m_mediator(mediator) {}

    Q_INVOKABLE void onGameVisible(QQuickItem *parentPage) {

        State* state = qobject_cast<State*>(State::instance());

        Matrix* theMatrix = parentPage->findChild<Matrix*>("matrix");
        theMatrix->setCenter(parentPage->width(), parentPage->height());

        m_mediator->reset();
        m_mediator->setMatrix(theMatrix);
        if (m_mediator && m_mediator->getList().size() == 0) {
            m_mediator->m_comm = state;
            for (int i = 0; i < GOBBLERS; i++) {

                int gobblerSize = i % 4; // 0 = HUGE, 1 = LARGE, 2 = MEDIUM, 3 = SMALL

                bool isBlack = i >= (GOBBLERS / 2);
                int xPosition = SIZE_TILE * 3.5 * (isBlack ? 1 : -1); // 3 Tiles from the center, and a half to center it.
                int j = isBlack ? i - (GOBBLERS / 2) : i;
                int zPosition = ((j / 4) - 1) * SIZE_TILE; // divide 12 into 3 piles

                Gobbler *gobblerItem = new Gobbler(parentPage, new Shape(), *new Gobbler3d(SIZE_TILE - PADDING - (gobblerSize * SIZE_DIFF)));

                if (gobblerItem) {
                    gobblerItem->m_matrix = theMatrix;

                    gobblerItem->setDepth(gobblerSize);
                    gobblerItem->setVisible(gobblerSize == 0);
                    gobblerItem->setSize(gobblerSize);
                    gobblerItem->setX3d(xPosition);
                    gobblerItem->setY3d(0);
                    gobblerItem->setZ3d(zPosition);

                    gobblerItem->setWhite(!isBlack);
                    //gobblerItem->setProperty("id", i);
                    //gobblerItem->setProperty("name", i);

                    gobblerItem->setWidth(parentPage->width());
                    gobblerItem->setHeight(parentPage->height());

                    m_mediator->addItem(gobblerItem);
                }
            }
        }
        if (state->isBlackTurn()) {
            m_mediator->startAi(true);
        }
    }

private:
    Mediator *m_mediator;
    static const int GOBBLERS = 24;
    static const int PADDING = 15;
    static const int SIZE_DIFF = 30;
    static const int SIZE_TILE = 150;
};
#endif // SETUPBOARD_H
