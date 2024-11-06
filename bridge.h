#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>

class Bridge : public QObject {
    Q_OBJECT

public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void incrementCounter();

public slots:
    void handleButtonClick() {
        emit incrementCounter();
    }
};

#endif // BRIDGE_H
