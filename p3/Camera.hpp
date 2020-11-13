#include "Scene.hpp"
#include "Direction.hpp"
#include "Point.hpp"

using namespace std;

class Camera{
    private:
        Direction* up, *left, *front; // Local camera coordinate system
        Point* origin; // Origin point
        int heigth, width; // Final image resolution
        int raysPP; // number of rays per pixel in ray tracing
    public:
        Camera(); 
        // The camera set its local directions automatically. Camera angle(of scene) aprox 30º
        // front = distance origin - scene center = 8 * (scene heigth/2)
        // module up = scene heigth
        // module left = front x up
        Camera(Scene scene); 
        void setOrigin(Point *p);
        void setResolution(int h, int w);
        void setCoord(Direction *u, Direction *l, Direction *f);
        void setRaysPP(int r);
        void rayTracing();
};