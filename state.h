#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QVector>
#include <qqmlengine.h>

class State : public QObject {
    Q_OBJECT
    Q_PROPERTY(int whiteCounter READ whiteCounter WRITE setWhiteCounter NOTIFY whiteCounterChanged)
    Q_PROPERTY(int blackCounter READ blackCounter WRITE setBlackCounter NOTIFY blackCounterChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(bool lock READ lock WRITE setLock NOTIFY lockChanged)

public:
    static QObject* instance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);

    bool isBlackTurn() const { return m_blackTurn; }

    int mode() const { return m_mode; }

    void setMode(int mode) {
        if (m_mode != mode) {
            m_mode = mode;
            emit modeChanged(m_mode);
        }
    }

    bool lock() const { return m_lock; }

    void setLock(bool value) {
        if (m_lock != value) {
            m_lock = value;
            emit lockChanged(m_lock);
        }
    }

    int whiteCounter() const {
        return m_whiteCounters[m_mode];
    }

    void setWhiteCounter(int value) {
        if (m_whiteCounters[m_mode] != value) {
            m_whiteCounters[m_mode] = value;
            emit whiteCounterChanged(value);
        }
    }

    int blackCounter() const {
        return m_blackCounters[m_mode];
    }

    void setBlackCounter(int value) {
        if (m_blackCounters[m_mode] != value) {
            m_blackCounters[m_mode] = value;
            emit blackCounterChanged(value);
        }
    }

signals:
    void whiteCounterChanged(int);
    void blackCounterChanged(int);
    void modeChanged(int);
    void lockChanged(bool);

public slots:
    void whiteWon() {
        m_blackTurn = !m_blackTurn;
        setWhiteCounter(whiteCounter() + 1);
    }

    void blackWon() {
        m_blackTurn = !m_blackTurn;
        setBlackCounter(blackCounter() + 1);
    }

private:
    explicit State(QObject *parent = nullptr);

    int m_mode = 0;
    int m_whiteCounters[3] = {0, 0, 0};
    int m_blackCounters[3] = {0, 0, 0};
    bool m_lock = false;
    bool m_blackTurn = false;
};

#endif // STATE_H
