#include "Equalize.hpp"

void Equalize::process(Image& img){
    RGB tuple;
    float color;
    for(int i = 0; i < img.getWidth(); i++){
        for(int j = 0; j < img.getHeight(); j++){
            tuple = img.getTuple(i, j);
            for(int k = 0; k < 3; k++){

                color = tuple.get(k) / img.getMaxFound();
                tuple.set(color, k);
                
                if(color > img.getMaxAfterMapping()){
                    img.setMaxAfterMapping(color);
                }
            }
            img.setTuple(tuple, i, j);
        }
    }
}