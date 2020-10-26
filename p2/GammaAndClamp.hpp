#ifndef GAMMACLAMPHPP
#define GAMMACLAMPHPP

#include <cmath>

#include "ToneMapper.hpp"
#include "Gamma.hpp"
#include "Clamp.hpp"

class GammaAndClamp: public ToneMapper{

    private:
        float g = 1.0;
        float V = 1.0;

    public:
        GammaAndClamp(float g, float V);
        void preprocess(Image& img);
        void process(Image& img);

};


#endif