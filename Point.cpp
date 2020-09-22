#include "Point.h"

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

Direction Point::operator-(Point const &operand){
    return Direction(c[0] - operand.getX(), c[1] - operand.getY(), c[2] - operand.getZ());
}