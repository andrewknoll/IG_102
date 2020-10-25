#ifndef GAMMAHPP
#define GAMMAHPP

#include <cmath>

#include "ToneMapper.hpp"

class Gamma: public ToneMapper{

    private:
        float g = 1.0;

    public:
        Gamma(float g);
        void process(Image& img);

};


#endif