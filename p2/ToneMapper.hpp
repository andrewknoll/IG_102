#ifndef TONEMAPPER
#define TONEMAPPER

#include "Image.hpp"
#include "RGB.hpp"

class ToneMapper{

    public:
        virtual void preprocess(Image& img){}
        virtual void process(Image& img) = 0;
        void apply(Image& img){
            preprocess(img);
            process(img);
        }

};


#endif