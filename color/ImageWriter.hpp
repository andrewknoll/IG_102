#ifndef __IMAGEWRITER__
#define __IMAGEWRITER__

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <string.h>
#include <memory>

#include "../imaging/Image.hpp"
#include "ToneMapper.hpp"


#include "Clamp.hpp"
#include "EqualizeAndClamp.hpp"
#include "GammaAndClamp.hpp"
#include "Reinhard2002.hpp"
#include "Reinhard2005.hpp"

using namespace std;
//Pointer to Tone Mapper
using ToneMapperPtr = shared_ptr<ToneMapper>;

//A single byte
typedef unsigned char byte;

//Enumeration with all the types of tone mapping techniques that can be applied to the images.
enum ToneMappingType : int{
    CLAMP=0, EQUALIZE=1, EQUALIZECLAMP=2, GAMMA=3, CLAMPGAMMA=4, REINHARD2002=5, REINHARD2005=6
};

//    Clamp:            setToneMapper(CLAMP, null)
//    Equalize:         setToneMapper(EQUALIZE, {imageMax})
//    EqualizeClamp:    setToneMapper(EQUALIZECLAMP, {V})
//    Gamma:            setToneMapper(GAMMA, {imageMax, gamma})
//    GammaClamp:       setToneMapper(CLAMPGAMMA, {V, gamma})
//    Reinhard2002:     setToneMapper(REINHARD2002, null)
//    Reinhard2005:     setToneMapper(REINHARD2005, {imageMax})
ToneMapperPtr createToneMapper(ToneMappingType tmt, float *args);

void writeImage(Image& img, string outputFile, ToneMappingType tmt, int destinationColorRes, bool bmp, float *toneMapperArgs);

#endif