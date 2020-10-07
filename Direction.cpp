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

Direction::Direction(Coordinates coord){
    c[0] = coord.getX();
    c[1] = coord.getY();
    c[2] = coord.getZ();
    c[3] = coord.getW();
}

float Direction::modulus(){
    return sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
}

//Normalize coordinates so modulus = 1
void Direction::normalize(){
    float mod = modulus();
    c[0] = c[0] / mod;
    c[1] = c[1] / mod;
    c[2] = c[2] / mod;
}