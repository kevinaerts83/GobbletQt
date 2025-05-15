#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>

class Bridge : public QObject {
    Q_OBJECT

public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}
    int mode() const { return m_mode; }
    bool isBlackTurn() const { return m_blackTurn; }
    bool lock() const { return m_lock; }

signals:
    void incrementWhite();
    void incrementBlack();
    void modeChanged(int newValue);
    void lockChanged(bool lock);

public slots:
    void whiteWon() {
        m_blackTurn = !m_blackTurn;
        emit incrementWhite();
    }
    void blackWon() {
        m_blackTurn = !m_blackTurn;
        emit incrementBlack();
    }
    void setMode(int value) {
        if (m_mode != value) {
            m_mode = value;
            emit modeChanged(m_mode);
        }
    }
    void setLock(bool value) {
        if (m_lock != value) {
            m_lock = value;
            emit lockChanged(m_lock);
        }
    }


private:
    int m_mode = 1;
    bool m_blackTurn = false;
    bool m_lock = false;
};

#endif // BRIDGE_H
