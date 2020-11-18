#include "vfield/Direction.hpp"
#include "vfield/Point.hpp"
#include "Shapes/Shape.hpp"
#include "Camera.hpp"

#include <list>

using namespace std;
using ShapeIterator = std::list<Shape>::iterator;

class Scene{
    private:
        int height, width; // Size (in pixels) of the projection plane
        list<Shape> list; // Geometries which compose the scene
        Camera camera;

        ShapeIterator si;

    public:
        Scene(int height, int width);
        Scene(int width, int height, Camera c);
        int getHeight();
        int getWidth();

        void setWidth(int width);
        void setHeight(int height);
        void setCamera(Camera camera);
        void buildCameraFromHFOV(int HFOV, Point target);
        void buildCameraFromVFOV(int HFOV, Point target);

        Point getTarget();
        Camera getCamera();

        void addShape(Shape s);

        Shape first();
        bool hasNext();
        Shape next();
        int length();
        
};
