#include "Direction.hpp"

#ifndef POINT
#define POINT

class Point : public Coordinates{

    public:
        Point();
        Point(Coordinates coord);
        Point(float x, float y, float z);
};

#endif