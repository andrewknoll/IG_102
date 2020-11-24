#ifndef __SHAPE__
#define __SHAPE__

#include "../color/RGB.hpp"
#include "../vfield/Operations.hpp"

class Shape{
    protected:
        RGB emission;

    public:
        virtual int findIntersectionWithLine(Direction d, Point o, Point result[])=0;
        RGB getEmission();
        void setEmission(RGB emission);

};

#endif