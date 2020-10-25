#include "EqualizeAndClamp.hpp"

EqualizeAndClamp::EqualizeAndClamp(float V){
    this->V = V;
}

void EqualizeAndClamp::preprocess(Image& img){
    Clamp clamping(V);
    clamping.apply(img);

    float maxAfterClamping = img.getMaxAfterMapping();

    img.setMaxFound(maxAfterClamping);

    //reset maxAfterMapping
    img.setMaxAfterMapping(0.0);
}

void EqualizeAndClamp::process(Image& img){

    Equalize equ;
    equ.apply(img);
}