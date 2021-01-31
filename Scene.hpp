#ifndef __SCENE__
#define __SCENE__

#include "vfield/Direction.hpp"
#include "vfield/Point.hpp"
#include "shapes/Shape.hpp"
#include "Camera.hpp"
#include "LightPoint.hpp"
#include "AreaLight.hpp"
#include <memory>

#include <vector>

using namespace std;

//***********************************************************************
// Scene for ray/path tracer implementation
//***********************************************************************
class Scene{
    private:
        int height, width; // Size (in pixels) of the projection plane
        vector<shared_ptr<Shape> > objects; // Geometries which compose the scene
        vector<LightPoint> lightPoints;
        vector<AreaLight> areaLights; 
        Camera camera;

    public:
        Scene(int height, int width);
        Scene(int width, int height, Camera c);
        int getHeight();
        int getWidth();

        void setWidth(int width);
        void setHeight(int height);
        void setCamera(Camera camera);
        void buildCameraFromHFOV(float HFOV, Point target);
        void buildCameraFromVFOV(float HFOV, Point target);

        Point getTarget();
        Camera getCamera();

        void addShape(shared_ptr<Shape> s);
        void addLight(LightPoint lp);
        void addLight(AreaLight al);

        shared_ptr<Shape> getShape(int i);
        LightPoint getLightPoint(int i);
        AreaLight getAreaLight(int i);
        int shapeN();
        int lightPN();
        int areaLightN();
        
};

#endif