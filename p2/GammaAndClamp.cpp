#include "GammaAndClamp.hpp"

GammaAndClamp::GammaAndClamp(float g, float V){
    this->V = V;
    this->g = g;
}

float GammaAndClamp::mapValue(float f){
    return f > V? 1.0 : pow(f/V, g);
}