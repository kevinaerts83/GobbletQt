#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>

class Bridge : public QObject {
    Q_OBJECT

public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void incrementWhite();
    void incrementBlack();

public slots:
    void whiteWon() {
        emit incrementWhite();
    }
    void blackWon() {
        emit incrementBlack();
    }
};

#endif // BRIDGE_H
