#ifndef __RAYTRACER__
#define __RAYTRACER__

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

using namespace std;

void rayTrace(Image& img, Scene scene, int rpp);

#endif