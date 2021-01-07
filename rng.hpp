#ifndef __RNG__
#define __RNG__

#include <random>

using namespace std;

struct{
    default_random_engine re;

    float getNumber(float min, float max){
        uniform_real_distribution<float> unif{min, max};
        return unif(re);
    }
} rng;

#endif