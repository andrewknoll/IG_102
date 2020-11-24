#ifndef __PLANE__
#define __PLANE__

#include "Shape.hpp"
#include "../vfield/Coordinates.hpp"
#include "../vfield/Operations.hpp"
#include "../vfield/Direction.hpp"
#include "../vfield/Point.hpp"
#include "../vfield/PossibleSolution.hpp"

class Plane : public Shape{
    protected:
        int distanceToOrigin;
        Direction normal;

    public:
        Plane(Direction normal, int distance, RGB emission);
        void setAll(float coord[3], int distance, float emission[3]);
        void findFirstIntersectionWithLine(Direction d, Point o, struct PossibleSolution<Point> result[]);
    
};

#endif