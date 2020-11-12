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