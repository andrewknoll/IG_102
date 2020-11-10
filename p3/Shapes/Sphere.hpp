#include "Shape.hpp"
#include "Coordinates.hpp"
#include "Direction.hpp"
#include "Point.hpp"

class Sphere : public Shape{
    protected:
        Direction axis;
        Point center;

    public:
        Sphere(Direction axis, Point center, RGB emission);
        void setCoords(float data[9]);
};