#ifndef __SHAPE__
#define __SHAPE__

#include "../color/RGB.hpp"
#include "../vfield/Operations.hpp"
#include "../vfield/PossibleSolution.hpp"

class Shape{
    protected:
        RGB emission;

    public:
        virtual void findFirstIntersectionWithLine(Direction d, Point o, struct PossibleSolution<Point> result[])=0;
        RGB getEmission();
        void setEmission(RGB emission);

};

#endif