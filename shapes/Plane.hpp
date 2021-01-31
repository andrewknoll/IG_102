#ifndef __PLANE__
#define __PLANE__

#include "Shape.hpp"
#include "../vfield/Coordinates.hpp"
#include "../vfield/Operations.hpp"
#include "../vfield/Direction.hpp"
#include "../vfield/Point.hpp"
#include "../globals.hpp"

//***********************************************************************
// Plane implementation class, from its normal and a distance to the world origin
//***********************************************************************
class Plane : public Shape{
    protected:
        int distanceToOrigin;
        Direction normal;

    public:
        Plane(Direction normal, int distance);
        void setAll(float coord[3], int distance);
        int findIntersectionWithLine(Direction d, Point o, float result[]);
        Direction getNormalAtPoint(Point p);
    
};

#endif