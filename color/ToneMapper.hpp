#ifndef __TONEMAPPER__
#define __TONEMAPPER__
/**************************************************************
 Abstract Tone Mapper base class
 **************************************************************/
class ToneMapper{

    public:
        // Abstract function to be overriden by any ToneMapper implementation.
        // @param f Value to be mapped
        // @returns the mapped value
        virtual float mapValue(float f) = 0;

};


#endif