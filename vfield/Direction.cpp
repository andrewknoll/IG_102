#include <cmath>
#include "Direction.hpp"

//***********************************************************************
// Class default constructor
// Calls "super" constructor
//***********************************************************************
Direction::Direction(){
    Coordinates();
}

//***********************************************************************
// Class constructor. Sets homogenous coordinate to 0
// @param x X coordinate
// @param y Y coordinate
// @param z Z coordinate
//***********************************************************************
Direction::Direction(float x, float y, float z){
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = 0.0;
}
//***********************************************************************
// Class constructor.
// @param coord Stores the values of the coordinates
//***********************************************************************
Direction::Direction(Coordinates coord){
    c[0] = coord.getX();
    c[1] = coord.getY();
    c[2] = coord.getZ();
    c[3] = coord.getW();
}

//***********************************************************************
// @returns Vector's module
//***********************************************************************
float Direction::modulus(){
    return sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
}

//***********************************************************************
// Normalizes vector
// The value of the modulus will be equal to 1
//***********************************************************************
void Direction::normalize(){
    float mod = modulus();
    c[0] = c[0] / mod;
    c[1] = c[1] / mod;
    c[2] = c[2] / mod;
}