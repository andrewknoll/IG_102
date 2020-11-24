#ifndef __SPHERE__
#define __SPHERE__

#include "Shape.hpp"
#include "../vfield/Coordinates.hpp"
#include "../vfield/Direction.hpp"
#include "../vfield/Point.hpp"
#include "../vfield/Operations.hpp"

class Sphere : public Shape{
    protected:
        Direction axis;
        Point center;

    public:
        Sphere(Direction axis, Point center, RGB emission);
        void setCoords(float data[9]);
        int findIntersectionWithLine(Direction d, Point o, Point result[]);
};

#endif