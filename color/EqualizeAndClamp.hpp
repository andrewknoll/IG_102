#ifndef __EQUALIZECLAMP__
#define __EQUALIZECLAMP__

#include "ToneMapper.hpp"

/**************************************************************
 Equalizing and clamping tone mapping techniques class
 **************************************************************/
class EqualizeAndClamp: public ToneMapper{

    private:
        //Maximum value that will not be clamped.
        //Higher values will be clamped to 1.
        float V = 1.0;

    public:
        EqualizeAndClamp(float V);
        float mapValue(float f);

};


#endif