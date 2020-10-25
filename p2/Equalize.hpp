#ifndef EQUALIZEHPP
#define EQUALIZEHPP

#include "ToneMapper.hpp"

class Equalize: public ToneMapper{

    public:
        void process(Image& img);

};


#endif