#ifndef GOBBLER_H
#define GOBBLER_H

#include <QtQuick/QQuickPaintedItem>
#include "shape.h"

class Gobbler : public Shape
{
public:
    Gobbler(QQuickItem *parent = 0);
};

#endif // GOBBLER_H
