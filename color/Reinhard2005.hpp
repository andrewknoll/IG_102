/*
/* Algorithm obtained from the following article:
/* https://64.github.io/tonemapping/
 */

#ifndef __REINHARD2005__
#define __REINHARD2005__

#include "ToneMapper.hpp"

/**************************************************************
 Reinhard tone mapping technique class (2005)
 **************************************************************/
class Reinhard2005: public ToneMapper{

    private:
        //Maximum value in the image to be mapped.
        float max;
        
    public:
        Reinhard2005(float max);
        float mapValue(float f);

};


#endif