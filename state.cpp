#include "state.h"

State::State(QObject *parent) : QObject(parent)
    , m_mode(1), m_whiteCounter(0), m_blackCounter(0), m_lock(false) {}

QObject* State::instance(QQmlEngine *, QJSEngine *) {
    static State *singleton = new State();
    return singleton;
}
