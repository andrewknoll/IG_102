#include "Direction.h"

#ifndef POINT
#define POINT

class Point : public Coordinates{

    public:
        Point();
        Point(float x, float y, float z);
        Direction operator-(Point const &operand);
};

#endif