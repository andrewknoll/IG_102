#include "Shape.hpp"
#include "Coordinates.hpp"
#include "Direction.hpp"
#include "Point.hpp"

class Plane : public Shape{
    protected:
        int distanceToOrigin;
        Direction normal;

    public:
        Plane(Direction normal, int distance, RGB emission);
        void setAll(float coord[6], int distance, float emission[3]);
    
};