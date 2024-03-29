#ifndef __SPHERE__
#define __SPHERE__

#include "Shape.hpp"
#include "../vfield/Coordinates.hpp"
#include "../vfield/Direction.hpp"
#include "../vfield/Point.hpp"
#include "../vfield/Operations.hpp"
//***********************************************************************
// Sphere implementation class, from an axis and a center
//***********************************************************************
class Sphere : public Shape{
    protected:
        Direction axis;
        Point center;

    public:
        Sphere(Direction axis, Point center);
        void setCoords(float data[6]);
        int findIntersectionWithLine(Direction d, Point o, float result[]);
        Direction getNormalAtPoint(Point p);
        void calculateTangentsAtPoint(Point p, Direction& t1, Direction& t2);

};

#endif