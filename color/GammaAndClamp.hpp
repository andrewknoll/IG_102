#ifndef __GAMMACLAMP__
#define __GAMMACLAMP__

#include <cmath>

#include "ToneMapper.hpp"

//Gamma(g) = GammaAndClamp(g, img.getMaxFound())
class GammaAndClamp: public ToneMapper{

    private:
        float g = 1.0;
        float V = 1.0;
        
    public:
        GammaAndClamp(float g, float V);
        float mapValue(float f);

};


#endif