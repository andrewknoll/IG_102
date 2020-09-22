#include "Coordinates.h"

#ifndef DIRECTION
#define DIRECTION

class Direction : public Coordinates{

    public:
        Direction();
        Direction(float x, float y, float z);

        double modulus();
};

#endif