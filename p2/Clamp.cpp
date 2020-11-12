#include "Clamp.hpp"

Clamp::Clamp(float V){
    this->V = V;
}

float Clamp::mapValue(float f){
    float res = f;
    if(f > this->V){
        res = 1.0;
    }
    return res;
}