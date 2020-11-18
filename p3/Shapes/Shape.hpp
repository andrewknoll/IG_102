#include "Color/RGB.hpp"
#include "vfield/Operations.hpp"

class Shape{
    protected:
        RGB emission;

    public:
        virtual PossibleSolution<Point> findFirstIntersectionWithLine(Direction d, Point o);
        RGB getEmission();
        void setEmission(RGB emission);

};