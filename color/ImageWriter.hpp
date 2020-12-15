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

using namespace std;
using ToneMapperPtr = shared_ptr<ToneMapper>;

typedef unsigned char byte;

enum ToneMappingType : int{
    CLAMP=0, EQUALIZE=1, EQUALIZECLAMP=2, GAMMA=3, CLAMPGAMMA=4
};

//Clamp:            setToneMapper(CLAMP, null)
//Equalize:         setToneMapper(EQUALIZE, {imageMax})
//EqualizeClamp:    setToneMapper(EQUALIZECLAMP, {V})
//Gamma:            setToneMapper(GAMMA, {imageMax, gamma})
//GammaClamp:       setToneMapper(CLAMPGAMMA, {V, gamma})
ToneMapperPtr createToneMapper(ToneMappingType tmt, float *args);

void writeImage(Image& img, string outputFile, ToneMappingType tmt, int destinationColorRes, bool bmp, float *toneMapperArgs);

#endif