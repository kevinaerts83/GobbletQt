#include "aimove.h"

aiMove::aiMove() {}

int aiMove::to() const {
    return m_to;
}
int aiMove::from() const {
    return m_from;
}

void aiMove::setTo(int value) {
    m_to = value;
}
void aiMove::setFrom(int value) {
    m_from = value;
}