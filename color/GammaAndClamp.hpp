#ifndef __GAMMACLAMP__
#define __GAMMACLAMP__

#include <cmath>

#include "ToneMapper.hpp"

/**************************************************************
 Gamma correction and clamping tone mapping techniques class
 **************************************************************/
class GammaAndClamp: public ToneMapper{

    private:
        //Gamma parameter
        float g = 1.0;
        //Maximum value that will not be clamped.
        //Higher values will be clamped to 1.
        float V = 1.0;
        
    public:
        GammaAndClamp(float g, float V);
        float mapValue(float f);

};


#endif