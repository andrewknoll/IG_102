#include "GammaAndClamp.hpp"

GammaAndClamp::GammaAndClamp(float g, float V){
    this->V = V;
    this->g = g;
}

void GammaAndClamp::preprocess(Image& img){
    EqualizeAndClamp equClamp(V);
    equClamp.apply(img);

    float maxAfterClamping = img.getMaxAfterMapping();

    img.setMaxFound(maxAfterClamping);

    //reset maxAfterMapping
    img.setMaxAfterMapping(0.0);


}

void GammaAndClamp::process(Image& img){
    Gamma gam(this->g);

    gam.apply(img);
}