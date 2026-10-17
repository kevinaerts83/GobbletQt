#include "stack3d.h"

Stack3d::Stack3d() : Shape3d() {

    double s = 15;

    this->points = {{-28*s,0,-20*s,1},
                    {-42*s,0,-20*s,1},
                    {-42*s,0,20*s,1},
                    {-28*s,0,20*s,1}};

    this->faces = {{0, 1, 2, 0},
                   {2, 3, 0, 0}};
}
