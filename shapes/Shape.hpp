#ifndef __SHAPE__
#define __SHAPE__

#include "../color/RGB.hpp"
#include "../materials/Material.hpp"
#include "../vfield/Operations.hpp"
#include <memory>

class Shape{
    protected:
        static long nextID;
        long myID;
        Material material;

    public:
        virtual int findIntersectionWithLine(Direction d, Point o, float result[])=0;
        virtual Direction getNormalAtPoint(Point p) = 0;
        virtual void calculateTangentsAtPoint(Point p, Direction& t1, Direction& t2)=0;
        Material getMaterial();
        void setMaterial(Material material);
        long getID();

        friend bool areEqual(shared_ptr<Shape> p1, shared_ptr<Shape> p2);
};

bool areEqual(shared_ptr<Shape> p1, shared_ptr<Shape> p2);

#endif