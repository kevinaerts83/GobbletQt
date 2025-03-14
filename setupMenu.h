#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <QObject>

class SetupMenu : public QObject {
    Q_OBJECT
    Q_PROPERTY(int whiteCounter READ whiteCounter WRITE setWhiteCounter NOTIFY whiteCounterChanged)
    Q_PROPERTY(int blackCounter READ blackCounter WRITE setBlackCounter NOTIFY blackCounterChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    //Q_PROPERTY(bool vertical READ isVertical WRITE setVertical NOTIFY verticalChanged)

public:
    explicit SetupMenu(QObject *parent = nullptr) : QObject(parent), m_whiteCounter(0), m_blackCounter(0), m_mode(1) {}

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

    /*bool isVertical() const { return m_vertical; }

    Q_INVOKABLE void setVertical(bool vertical) {
        m_vertical = !vertical;
        emit verticalChanged();
    }*/

    Q_INVOKABLE void setLock() {
        emit lockChanged();
    }

signals:
    void whiteCounterChanged();
    void blackCounterChanged();
    void modeChanged();
    //void verticalChanged();
    void lockChanged();

private:
    int m_whiteCounter;
    int m_blackCounter;
    int m_mode;
    //bool m_vertical = true;
};

#endif // SETUPMENU_H
