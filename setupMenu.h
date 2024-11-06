#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <QObject>

class SetupMenu : public QObject {
    Q_OBJECT
    Q_PROPERTY(int whiteCounter READ whiteCounter WRITE setWhiteCounter NOTIFY whiteCounterChanged)
    Q_PROPERTY(int blackCounter READ blackCounter WRITE setBlackCounter NOTIFY blackCounterChanged)

public:
    explicit SetupMenu(QObject *parent = nullptr) : QObject(parent), m_whiteCounter(0), m_blackCounter(0) {}

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

signals:
    void whiteCounterChanged();
    void blackCounterChanged();

private:
    int m_whiteCounter;
    int m_blackCounter;
};

#endif // SETUPMENU_H
