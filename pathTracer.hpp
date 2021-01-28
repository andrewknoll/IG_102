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
        int current_width  = 0;
        int current_height = 0;

        mutex mtx;
        condition_variable cv;
        bool ready = true;

        atomic_int pixelsLeftToGenerate;

        void threadWork(Image& img, Scene scene, int rpp, int subdivisions);
        void applyToSubimage(Image& img, Scene scene, int rpp, int subdivisions);

        int numberOfSubdivisions(int width, int height);
        void getSubdivision(const int w, const int h, const int n, int& w0, int& wf, int& h0, int& hf);

    public:
        void pathTrace(Image& img, Scene scene, int rpp);
};
#endif