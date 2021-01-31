#include "EqualizeAndClamp.hpp"

//***********************************************************************
//  Class constructor.
//  @param V Maximum value that will not be clamped.
//***********************************************************************
EqualizeAndClamp::EqualizeAndClamp(float V){
    this->V = V;
}

//***********************************************************************
//  Applies the equalizing and clamping tone mapping techniques
//  (values will be normalized and values higher than field V will be set to 1)
//  @param f Value to be mapped
//  @returns the mapped value
//***********************************************************************
float EqualizeAndClamp::mapValue(float f){
    float res;
    if(f > this->V){
        res = 1.0;
    }
    else{
        res = f / this->V;
    }
    return res;
    
}