#ifndef __RNG__
#define __RNG__

#include <random>

using namespace std;

struct randomNumberGen{
    default_random_engine re;

    double getNumber(double min, double max){
        uniform_real_distribution<double> unif{min, max};
        return unif(re);
    }
};

#endif