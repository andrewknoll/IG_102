#ifndef __CLAMP__
#define __CLAMP__

#include "ToneMapper.hpp"

class Clamp: public ToneMapper{

    private:
        float V = 1.0;
        
    public:
        Clamp(float V = 1.0);
        float mapValue(float f);

};


#endif