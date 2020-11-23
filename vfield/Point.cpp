#include "Point.hpp"

Point::Point(){
    Coordinates();
    c[3] = 1.0;
}

Point::Point(Coordinates coord){
    c[0] = coord.getX();
    c[1] = coord.getY();
    c[2] = coord.getZ();
    c[3] = coord.getW();
}

Point::Point(float x, float y, float z){
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = 1.0;
}