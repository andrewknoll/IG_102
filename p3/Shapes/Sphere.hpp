#include "Shape.hpp"
#include "vfield/Coordinates.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Point.hpp"
#include "vfield/Operations.hpp"

class Sphere : public Shape{
    protected:
        Direction axis;
        Point center;

    public:
        Sphere(Direction axis, Point center, RGB emission);
        void setCoords(float data[9]);
        PossibleSolution<Point> findFirstIntersectionWithLine(Direction d, Point o);
};