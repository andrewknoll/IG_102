#include "Coordinates.hpp"

#ifndef __DIRECTION__
#define __DIRECTION__

//***********************************************************************
// 3x3 Direction / Vector implementation (with Homogenous coordinate)
//***********************************************************************
class Direction : public Coordinates{

    public:
        Direction();
        Direction(Coordinates coord);
        Direction(float x, float y, float z);

        float modulus();
        void normalize();
};

#endif