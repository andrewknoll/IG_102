#include "Clamp.hpp"

Clamp::Clamp(float V){
    this->V = V;
}

void Clamp::process(Image& img){
    RGB tuple;
    float color;
    for(int i = 0; i < img.getHeight(); i++){
        for(int j = 0; j < img.getWidth(); j++){
            tuple = img.getTuple(i, j);
            for(int k = 0; k < 3; k++){
                color = tuple.get(k);
                if(color > this->V){
                    tuple.set(1.0, k);
                }
                color = tuple.get(k);
                if(color > img.getMaxAfterMapping()){
                    img.setMaxAfterMapping(color);
                }
            }
            img.setTuple(tuple, i, j);
        }
    }
}