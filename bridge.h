#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>

class Bridge : public QObject {
    Q_OBJECT

public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}
    int mode() const { return m_mode; }
    //bool vertical() const { return m_vertical; }
    bool lock() const { return m_lock; }

signals:
    void incrementWhite();
    void incrementBlack();
    void modeChanged(int newValue);
    //void verticalChanged(bool vertical);
    void lockChanged(bool lock);

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
    //void setVertical(bool vertical) {
    //    if (m_vertical != vertical) {
    //        m_vertical = vertical;
    //        emit verticalChanged(m_vertical);
    //    }
    //}
    void setLock(bool value) {
        if (m_lock != value) {
            m_lock = value;
            emit lockChanged(m_lock);
        }
    }


private:
    int m_mode = 1;
    //bool m_vertical = true;
    bool m_lock = false;
};

#endif // BRIDGE_H
