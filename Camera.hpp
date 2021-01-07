#ifndef __CAMERA__
#define __CAMERA__

#include "vfield/Direction.hpp"
#include "vfield/Coordinates.hpp"
#include "vfield/Operations.hpp"
#include "vfield/Point.hpp"
#include <math.h>

using namespace std;

class Camera{
    private:
        Direction up, left, front; // Local camera coordinate system
        Point origin; // Origin point

    public:
        Camera();
        Camera(int width, int height, Point target, float FOV, bool hfov);
        Camera(Direction u, Direction l, Direction f, Point o);
        void setOrigin(Point p);
        void setResolution(int h, int w);
        void setCoord(Direction u, Direction l, Direction f);

        void setAspectRatio(int width, int height);
        void setHFOV(float fov);
        void setVFOV(float fov);

        Direction getUp();
        Direction getLeft();
        Direction getFront();
        Point getOrigin();
        
        Point changeToGlobalCoordinates(Point p);
        Direction changeToGlobalCoordinates(Direction d);
};

#endif