#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <QObject>

class SetupMenu : public QObject {
    Q_OBJECT
    Q_PROPERTY(int whiteCounter READ whiteCounter WRITE setWhiteCounter NOTIFY whiteCounterChanged)
    Q_PROPERTY(int blackCounter READ blackCounter WRITE setBlackCounter NOTIFY blackCounterChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(bool lock READ lock WRITE setLock NOTIFY lockChanged)

public:
    explicit SetupMenu(QObject *parent = nullptr) : QObject(parent), m_whiteCounter(0), m_blackCounter(0), m_mode(1), m_lock(false) {}

    int whiteCounter() const { return m_whiteCounter; }

    void setWhiteCounter(int count) {
        if (m_whiteCounter != count) {
            m_whiteCounter = count;
            emit whiteCounterChanged();
        }
    }

    int blackCounter() const { return m_blackCounter; }

    void setBlackCounter(int count) {
        if (m_blackCounter != count) {
            m_blackCounter = count;
            emit blackCounterChanged();
        }
    }

    int mode() const { return m_mode; }

    Q_INVOKABLE void setMode(int mode) {
        if (m_mode != mode) {
            m_mode = mode;
            emit modeChanged();
        }
    }

    bool lock() const { return m_lock; }

    Q_INVOKABLE void setLock(bool lock) {
        if (m_lock != lock) {
            m_lock = lock;
            emit lockChanged();
        }
    }

signals:
    void whiteCounterChanged();
    void blackCounterChanged();
    void modeChanged();
    void lockChanged();

private:
    int m_whiteCounter;
    int m_blackCounter;
    int m_mode;
    bool m_lock;
};

#endif // SETUPMENU_H
