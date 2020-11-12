#ifndef __EQUALIZECLAMP__
#define __EQUALIZECLAMP__

#include "ToneMapper.hpp"

//Equalize() = EqualizeAndClamp(img.getMaxFound())
class EqualizeAndClamp: public ToneMapper{

    private:
        float V = 1.0;

    public:
        EqualizeAndClamp(float V);
        float mapValue(float f);

};


#endif