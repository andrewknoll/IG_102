#include "Direction.hpp"

#ifndef __POINT__
#define __POINT__

class Point : public Coordinates{

    public:
        Point();
        Point(Coordinates coord);
        Point(float x, float y, float z);
};

#endif