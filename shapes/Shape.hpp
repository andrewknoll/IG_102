#ifndef __SHAPE__
#define __SHAPE__

#include "../color/RGB.hpp"
#include "../materials/Material.hpp"
#include "../vfield/Operations.hpp"

class Shape{
    protected:
        Material material;

    public:
        virtual int findIntersectionWithLine(Direction d, Point o, double result[])=0;
        virtual Direction getNormalAtPoint(Point p) = 0;
        virtual void calculateTangentsAtPoint(Point p, Direction& t1, Direction& t2)=0;
        Material getMaterial();
        void setMaterial(Material material);
};

#endif