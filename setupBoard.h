#ifndef SETUPBOARD_H
#define SETUPBOARD_H

#include <QObject>

#include "bridge.h"
#include "mediator.h"
#include "gobbler.h"

class SetupBoard : public QObject {
    Q_OBJECT
public:
    explicit SetupBoard(Bridge *comm, QObject *parent = nullptr)
        : QObject(parent), m_comm(comm) {}

    Q_INVOKABLE void onGameVisible(QQuickItem *parentPage) {

        Mediator* mediator = parentPage->findChild<Mediator*>("mediator");
        if (mediator && mediator->getList().size() == 0) {
            mediator->m_comm = m_comm;
            for (int i = 0; i < 24; i++) {
                Gobbler *gobblerItem = new Gobbler(parentPage, new Shape(), *new Gobbler3d(150 - 15 - ((i % 4) * 30)));

                if (gobblerItem) {
                    gobblerItem->m_matrix = parentPage->findChild<Matrix*>("matrix");
                    //gobblerItem->model = *new Gobbler3d(150);

                    gobblerItem->setDepth(i % 4);
                    gobblerItem->setVisible((i % 4) == 0);
                    gobblerItem->setSize(i % 4);
                    gobblerItem->setX3d((i > 11) ? 375 : -375);
                    gobblerItem->setY3d(0);
                    gobblerItem->setZ3d(-150 + ((((i > 11) ? i - 12 : i)/ 4) * 150));

                    gobblerItem->setWhite(i < 12);
                    gobblerItem->setProperty("id", i);
                    gobblerItem->setProperty("name", i);

                    gobblerItem->setWidth(1024);
                    gobblerItem->setHeight(768);

                    mediator->addItem(gobblerItem);
                }
            }
        }
    }

private:
    Bridge *m_comm;
};
#endif // SETUPBOARD_H
