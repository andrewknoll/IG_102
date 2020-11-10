#include "Coordinates.hpp"

#ifndef DIRECTION
#define DIRECTION

class Direction : public Coordinates{

    public:
        Direction();
        Direction(Coordinates coord);
        Direction(float x, float y, float z);

        float modulus();
        void normalize();
};

#endif