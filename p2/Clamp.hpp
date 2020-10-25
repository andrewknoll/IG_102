#ifndef CLAMPHPP
#define CLAMPHPP

#include "ToneMapper.hpp"
#include "Image.hpp"
#include "RGB.hpp"

class Clamp: public ToneMapper{

    private:
        float V = 1.0;

    public:
        Clamp(float V = 1.0);
        void process(Image& img);

};


#endif