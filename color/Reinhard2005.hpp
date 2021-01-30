#ifndef __REINHARD2005__
#define __REINHARD2005__

#include "ToneMapper.hpp"

class Reinhard2005: public ToneMapper{

    private:
        float max;
        
    public:
        Reinhard2005(float max);
        float mapValue(float f);

};


#endif