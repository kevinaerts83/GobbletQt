#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>

class Bridge : public QObject {
    Q_OBJECT

public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}
    int mode() const { return m_mode; }

signals:
    void incrementWhite();
    void incrementBlack();
    void modeChanged(int newValue);

public slots:
    void whiteWon() {
        emit incrementWhite();
    }
    void blackWon() {
        emit incrementBlack();
    }
    void setMode(int value) {
        if (m_mode != value) {
            m_mode = value;
            emit modeChanged(m_mode);
        }
    }

private:
    int m_mode = 1;
};

#endif // BRIDGE_H
