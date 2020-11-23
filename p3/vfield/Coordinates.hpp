#ifndef __COORDINATES__
#define __COORDINATES__

#include <iostream>
#include <cassert>
#include "Matrix4.hpp"

using namespace std;

class Coordinates{
    protected:
        float c[4];

    public:
        Coordinates();
        Coordinates(float x, float y, float z, float w);

        virtual float getX() const;
        virtual float getY() const;
        virtual float getZ() const;
        virtual float getW() const;

        virtual void setX(float x);
        virtual void setY(float y);
        virtual void setZ(float z);
        virtual void setW(float w);

        virtual void setAll(float x, float y, float z);
        virtual const float* getVector() const;

        virtual float& operator[](int i);
        virtual float  operator[](int i) const;
        virtual float& operator[](char c);
        virtual float  operator[](char c) const;

        virtual Coordinates operator* (Matrix4 m) const;
};

ostream& operator<<(ostream& os, const Coordinates& coord);

#endif