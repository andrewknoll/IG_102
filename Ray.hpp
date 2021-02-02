#ifndef __RAY__
#define __RAY__

#include <memory>
#include <limits>
#include "shapes/Shape.hpp"
#include "materials/Material.hpp"
#include "Scene.hpp"
#include "AreaLight.hpp"
#include "globals.hpp"

using ShapePtr = shared_ptr<Shape>;
using namespace std;

//Maximum float value
const float MAX_float = numeric_limits<float>::max();

//***********************************************************************
// Ray implementation
//***********************************************************************
class Ray{
    private:
        Point origin;
        Direction dir;
    public:
        Direction getDirection();
        void setDirection(Direction dir);
        void setOrigin(Point origin);

        int findIntersectionWith(ShapePtr shape, float solutions[]);
        RGB getRayResult(Scene& scene);
        bool getShadowRayResult(Scene& scene, RGB& initial, LightPoint destination, ShapePtr lastShape);
        Event castShadowRays(Scene& scene, RGB& initial, Point origin, ShapePtr lastShape);
};

#endif