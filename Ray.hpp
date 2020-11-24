#include <memory>
#include <limits>
#include "shapes/Shape.hpp"
#include "Scene.hpp"


using ShapePtr = shared_ptr<Shape>;
using namespace std;

const double MAX_DOUBLE = numeric_limits<double>::max();


class Ray{
    private:
        Point origin;
        Direction dir;

    public:
        void setDirection(Direction dir);
        void setOrigin(Point origin);

        int findIntersectionWith(ShapePtr shape, Point solutions[]);
        RGB getRayResult(Scene& scene);
};
