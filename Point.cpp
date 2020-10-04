#include "Point.hpp"

Point::Point(){
    Coordinates();
    c[3] = 1.0;
}

Point::Point(float x, float y, float z){
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = 1.0;
}