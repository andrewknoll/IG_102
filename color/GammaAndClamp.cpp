#include "GammaAndClamp.hpp"

//***********************************************************************
//  Class constructor.
//  @param g Gamma parameter for gamma correction
//  @param V Maximum value that will not be clamped.
//***********************************************************************
GammaAndClamp::GammaAndClamp(float g, float V){
    this->V = V;
    this->g = g;
}

//***********************************************************************
//  Applies the gamma correction and clamping tone mapping techniques
//  (values will be normalized, with gamma correction applied and values higher than field V will be set to 1)
//  @param f Value to be mapped
//  @returns the mapped value
//***********************************************************************
float GammaAndClamp::mapValue(float f){
    return f > V? 1.0 : pow(f/V, g);
}