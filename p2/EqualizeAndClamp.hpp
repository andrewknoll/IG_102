#ifndef EQUALIZECLAMPHPP
#define EQUALIZECLAMPHPP

#include "ToneMapper.hpp"
#include "Clamp.hpp"
#include "Equalize.hpp"

class EqualizeAndClamp: public ToneMapper{

    private:
        float V = 1.0;

    public:
        EqualizeAndClamp(float frontier);
        void preprocess(Image& img);
        void process(Image& img);

};


#endif