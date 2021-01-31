#include "RGB.hpp"

//***********************************************************************
// Default contructor.
// Initial values are (0,0,0)
//***********************************************************************
RGB::RGB(){
    red = 0;
    green = 0;
    blue = 0;
}

//***********************************************************************
// Class constructor.
// @param red Red value of the triplet.
// @param green Green value of the triplet.
// @param blue Blue value of the triplet.
//***********************************************************************
RGB::RGB(float red, float green, float blue){
    this->red = red;
    this->green = green;
    this->blue = blue;
}

//***********************************************************************
// Prints a string with format (R, G, B)
// through standard output for debug purposes
//***********************************************************************
void RGB::toString(){
    cout << red << " " << green << " " << blue;
}

//***********************************************************************
// Returns the value of specified component of the triplet
// @param i Index of the component
// @returns Value of the i'th component of the triplet
//***********************************************************************
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

//***********************************************************************
// Returns a reference to the specified component of the triplet
// @param i Index of the component
// @returns A reference to the i'th component of the triplet. If i > 3, returns re1
//***********************************************************************
float& RGB::operator[](const int i){
    switch(i){
        case 0:
            return red;
        case 1:
            return green;
        case 2:
            return blue;
    }
    return red;
}

//***********************************************************************
// Resets triplet values to (0,0,0)
//***********************************************************************
void RGB::reset(){
    red = 0;
    green = 0;
    blue = 0;
}

//***********************************************************************
// Returns max value
// @returns maximum value of the triplet.
//***********************************************************************
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

//***********************************************************************
// Add operator.
// @param b Second operand
// @returns A triplet representing the sum of both the triplets
//***********************************************************************
RGB RGB::operator+(RGB b){
    return RGB(red+b[0], green+b[1], blue+b[2]);
}

//***********************************************************************
// Substract operator.
// @param b Second operand
// @returns A triplet representing the substraction of the left operand and the right.
//***********************************************************************
RGB RGB::operator-(RGB b){
    return RGB(red-b[0], green-b[1], blue-b[2]);
}

//***********************************************************************
// Product operator.
// @param b Second operand
// @returns A triplet representing whose values are the products of the components one to one of both operands
//***********************************************************************
RGB RGB::operator*(RGB b){
    return RGB(red*b[0], green*b[1], blue*b[2]);
}

//***********************************************************************
// Product operator with a real value.
// @param b Real value (Second operand)
// @returns A triplet representing the product of all the components by the real value.
//***********************************************************************
RGB RGB::operator*(float b){
    return RGB(red*b, green*b, blue*b);
}

//***********************************************************************
// Division operator with a real value.
// @param b Real value (Second operand)
// @returns A triplet representing the division of all the components by the real value.
//***********************************************************************
RGB RGB::operator/(float b){
    return RGB(red/b, green/b, blue/b);
}

//***********************************************************************
// Division operator.
// @param b Second operand
// @returns A triplet representing whose values are the divisions of the components one to one of both operands
//***********************************************************************
RGB RGB::operator/(RGB b){
    return RGB(red/b[0], green/b[1], blue/b[2]);
}

//***********************************************************************
// Returns the triplet representing the mean of the values of the all the triplets
// @param v Vector containing all the RGB tuples
// @returns average value of all the RGB tuples
// @relatealso RGB
//***********************************************************************
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