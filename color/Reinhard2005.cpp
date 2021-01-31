/*
 * Algorithm obtained from paper (Retrieved 15 December 2020)
 * https://64.github.io/tonemapping/#extended-reinhard
 *
 */
#include "Reinhard2005.hpp"

//***********************************************************************
// Class constructor.
//  @param max Maximum light value in the image to be mapped.
//***********************************************************************
Reinhard2005::Reinhard2005(float max){
    this->max = max;
}

//***********************************************************************
//  Applies the Reinhard tone mapping technique (2005).
//  @param f Value to be mapped
//  @returns the mapped value
//***********************************************************************
float Reinhard2005::mapValue(float f){
    return (f * (1 + (f / (max * max)))) / (1 + f);
}