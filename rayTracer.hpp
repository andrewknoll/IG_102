#ifndef __RAYTRACER__
#define __RAYTRACER__

#include "Scene.hpp"
#include "imaging/Image.hpp"
#include "color/RGB.hpp"
#include "shapes/Shape.hpp"
#include "vfield/Point.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Operations.hpp"
#include <random>
#include <vector>

using namespace std;

struct randomNumberGen{
    default_random_engine re;
    uniform_real_distribution<double> unif{0, 1};

    double getNumber(){
        return unif(re);
    }
};

void rayTrace(Image& img, Scene scene, int rpp);

#endif