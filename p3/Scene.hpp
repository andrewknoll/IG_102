#include "Direction.hpp"
#include "Point.hpp"
#include "Shape.hpp"
#include <vector>

using namespace std;

class Scene{
    private:
        Point center; // Punto central de la escena
        int heigth, width; // maxZ - center[z], maxY(p.e) - center[y]
        vector<Shape> scene; // Geometries which compose the scene

    public:
        Scene(Shape s);
        Scene(vector<Shape> s, Point c);
        Point getCenter();
        int getHeigth();
        int getWidth();
};
