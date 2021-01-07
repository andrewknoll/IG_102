#ifndef __AREALIGHT__
#define __AREALIGHT__
#include "color/RGB.hpp"
#include "vfield/Point.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Operations.hpp"

/*
            ____
 height___|  .  |
 origin   |_____|
             |base
*/


class AreaLight{
    private:
        RGB emission;
        Point origin;
        //Must be normalized
        Direction base, height;
        float boundU, boundV;

    public:
        AreaLight();
        AreaLight(Point origin, Direction base, Direction height, float boundU, float boundV, RGB emission);
        void setEmission(RGB emission);
        RGB getEmission();

        void setOrigin(Point origin);
        Point getOrigin();

        void setBase(Direction base);
        Direction getBase();

        void setHeight(Direction height);
        Direction getHeight();

        int findIntersectionWithLine(Direction d, Point o, float result[]);


};
#endif