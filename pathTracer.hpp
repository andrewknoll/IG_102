#ifndef __PATHTRACER__
#define __PATHTRACER__

#include "rng.hpp"
#include "Scene.hpp"
#include "imaging/Image.hpp"
#include "color/RGB.hpp"
#include "shapes/Shape.hpp"
#include "vfield/Point.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Operations.hpp"
#include "Ray.hpp"
#include <random>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <thread>
#include <math.h>
#include <mutex>
#include <atomic>
#include <condition_variable>

using namespace std;

class PathTracer{
    private:
        atomic_int current_width  = {0};
        atomic_int current_height = {0};

        atomic_int pixelsLeftToGenerate;

        void threadWork(Image& img, Scene scene, int rpp, int subdivisions, int id);
        void applyToSubimage(Image& img, Scene scene, int rpp, int subdivisions, int id);

        int numberOfSubdivisions(int width, int height);

    public:
        void pathTrace(Image& img, Scene scene, int rpp);
};
#endif