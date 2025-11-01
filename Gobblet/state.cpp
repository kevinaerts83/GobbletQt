#include "state.h"

State::State(QObject *parent)
    : QObject(parent), m_mode(1), m_lock(false), m_blackTurn(false) {

    // Initialize the arrays
    for (int i = 0; i < 3; ++i) {
        m_whiteCounters[i] = 0;
        m_blackCounters[i] = 0;
    }
}

QObject* State::instance(QQmlEngine *, QJSEngine *) {
    static State *singleton = new State();
    return singleton;
}
