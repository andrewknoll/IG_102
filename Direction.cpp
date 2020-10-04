#include <cmath>
#include "Direction.hpp"

Direction::Direction(){
    Coordinates();
}

Direction::Direction(float x, float y, float z){
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = 0.0;
}

float Direction::modulus(){
    return sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
}