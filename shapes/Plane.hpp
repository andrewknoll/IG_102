#ifndef __PLANE__
#define __PLANE__

#include "Shape.hpp"
#include "../vfield/Coordinates.hpp"
#include "../vfield/Operations.hpp"
#include "../vfield/Direction.hpp"
#include "../vfield/Point.hpp"

class Plane : public Shape{
    protected:
        int distanceToOrigin;
        Direction normal;

    public:
        Plane(Direction normal, int distance);
        void setAll(float coord[3], int distance);
        int findIntersectionWithLine(Direction d, Point o, double result[]);
        Direction getNormalAtPoint(Point p);
        void calculateTangentsAtPoint(Point p, Direction& t1, Direction& t2);

        Point getRandomPoint();
    
};

#endif