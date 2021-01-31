#ifndef __CLAMP__
#define __CLAMP__

#include "ToneMapper.hpp"
/**************************************************************
 Clamping tone mapping technique class
 **************************************************************/
class Clamp: public ToneMapper{

    private:
        //Maximum value that will not be clamped.
        //Higher values will be clamped to 1.
        float V = 1.0;
        
    public:
        Clamp(float V = 1.0);
        float mapValue(float f);

};


#endif