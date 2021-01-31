#include "Clamp.hpp"

//***********************************************************************
// Default constructor.
//  @param V Maximum value that will not be clamped. Default value = 1.0
//***********************************************************************
Clamp::Clamp(float V){
    this->V = V;
}

//***********************************************************************
//  Applies the clamping tone mapping technique.
//  (values higher than field V will be set to 1)
//  @param f Value to be mapped
//  @returns the mapped value
//***********************************************************************
float Clamp::mapValue(float f){
    float res = f;
    if(f > this->V){
        res = 1.0;
    }
    return res;
}