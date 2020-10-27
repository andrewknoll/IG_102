#include "EqualizeAndClamp.hpp"

EqualizeAndClamp::EqualizeAndClamp(float V){
    this->V = V;
}

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