#ifndef __RNG__
#define __RNG__

#include <cstdlib>

using namespace std;

struct{
    float getNumber(float min, float max){
        return (rand() * 1.0 / RAND_MAX) * max + min;
    }
} rng;

#endif