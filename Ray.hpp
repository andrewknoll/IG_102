#ifndef __RAY__
#define __RAY__

#include <memory>
#include <limits>
#include "shapes/Shape.hpp"
#include "materials/Material.hpp"
#include "Scene.hpp"
#include "AreaLight.hpp"


using ShapePtr = shared_ptr<Shape>;
using namespace std;

const double MAX_DOUBLE = numeric_limits<double>::max();


class Ray{
    private:
        Point origin;
        Direction dir;
    public:
        Direction getDirection();
        void setDirection(Direction dir);
        void setOrigin(Point origin);

        int findIntersectionWith(ShapePtr shape, double solutions[]);
        RGB getRayResult(Scene& scene);
        Event getShadowRayResult(Scene& scene, RGB& initial, LightPoint destination, ShapePtr lastShape);
        Event castShadowRays(Scene& scene, RGB& initial, Point origin, ShapePtr lastShape);
};

#endif