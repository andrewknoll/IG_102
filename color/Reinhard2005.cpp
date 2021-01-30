/*
 * Algorithm obtained from paper (Retrieved 15 December 2020)
 * https://64.github.io/tonemapping/#extended-reinhard
 *
 */
#include "Reinhard2005.hpp"

Reinhard2005::Reinhard2005(float max){
    this->max = max;
}

float Reinhard2005::mapValue(float f){
    return (f * (1 + (f / (max * max)))) / (1 + f);
}