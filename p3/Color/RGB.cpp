#include "RGB.hpp"

RGB::RGB(){
    red = 0;
    green = 0;
    blue = 0;
}

RGB::RGB(float red, float green, float blue){
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void RGB::toString(){
    cout << red << " " << green << " " << blue;
}

float RGB::get(int i){
    float res = 0;
    switch(i){
        case 0:
            res = red;
            break;
        case 1:
            res = green;
            break;
        case 2:
            res = blue;
            break;
    }
    return res;
}

void RGB::set(float c, int i){
    switch(i){
        case 0:
            red = c;
            break;
        case 1:
            green = c;
            break;
        case 2:
            blue = c;
            break;
    }
}

RGB calculateRGBMean(vector<RGB> v){
    float results[3];
    int size = v.size();
    for(int i = 0; i < size; i++){
        for(int c = 0; c < 3; c++){
            results[c] += v[i].get(c);
        }
    }
    return RGB(results[0] / size, results[1] / size, results[2] / size);
}