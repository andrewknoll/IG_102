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

float RGB::operator[](const int i) const{
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

float& RGB::operator[](const int i){
    switch(i){
        case 0:
            return red;
        case 1:
            return green;
        case 2:
            return blue;
    }
}

void RGB::reset(){
    red = 0;
    green = 0;
    blue = 0;
}

float RGB::max(){
    float r = red;
    if(r < green){
        r = green;
    }
    if(r < blue){
        r = blue;
    }
    return r;
}

RGB calculateRGBMean(vector<RGB> v){
    float results[3] = {0, 0, 0};
    int size = v.size();
    for(int i = 0; i < size; i++){
        for(int c = 0; c < 3; c++){
            results[c] += v[i][c];
        }
    }
    return RGB(results[0] / size, results[1] / size, results[2] / size);
}

//operator used for coefficient sums
RGB RGB::operator+(RGB b){
    return RGB(red+b[0], green+b[1], blue+b[2]);
}

//operator used for coefficient sums
RGB RGB::operator-(RGB b){
    return RGB(red-b[0], green-b[1], blue-b[2]);
}

//operator used for coefficient products
RGB RGB::operator*(RGB b){
    return RGB(red*b[0], green*b[1], blue*b[2]);
}

//operator used for coefficient products
RGB RGB::operator*(float b){
    return RGB(red*b, green*b, blue*b);
}

//operator used for coefficient products
RGB RGB::operator/(float b){
    return RGB(red/b, green/b, blue/b);
}

//operator used for coefficient products
RGB RGB::operator/(RGB b){
    return RGB(red/b[0], green/b[1], blue/b[2]);
}