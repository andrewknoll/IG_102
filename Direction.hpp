#include "Coordinates.hpp"

#ifndef DIRECTION
#define DIRECTION

class Direction : public Coordinates{

    public:
        Direction();
        Direction(float x, float y, float z);

        float modulus();
};

#endif