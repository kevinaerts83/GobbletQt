#ifndef STATE_H
#define STATE_H

#include <QObject>
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

    int whiteCounter() const { return m_whiteCounter; }

    void setWhiteCounter(int count) {
        if (m_whiteCounter != count) {
            m_whiteCounter = count;
            emit whiteCounterChanged(m_whiteCounter);
        }
    }

    int blackCounter() const { return m_blackCounter; }

    void setBlackCounter(int count) {
        if (m_blackCounter != count) {
            m_blackCounter = count;
            emit blackCounterChanged(m_blackCounter);
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
        setWhiteCounter(m_whiteCounter + 1);
    }
    void blackWon() {
        m_blackTurn = !m_blackTurn;
        setBlackCounter(m_blackCounter + 1);
    }

private:
    explicit State(QObject *parent = nullptr);
    int m_mode;
    int m_whiteCounter;
    int m_blackCounter;
    bool m_lock;
    bool m_blackTurn = false;
};

#endif // STATE_H
