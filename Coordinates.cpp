#include "Coordinates.h"

Coordinates::Coordinates(){
    c[0] = 0.0;
    c[1] = 0.0;
    c[2] = 0.0;
    c[3] = 0.0;
}

Coordinates::Coordinates(float x, float y, float z, float w){
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
}

float Coordinates::getX() const{
    return c[0];
}

float Coordinates::getY() const{
    return c[1];
}

float Coordinates::getZ() const{
    return c[2];
}

float Coordinates::getW() const{
    return c[3];
}

void Coordinates::setX(float x){
    c[0] = x;
}

void Coordinates::setY(float y){
    c[1] = y;
}

void Coordinates::setZ(float z){
    c[2] = z;
}

void Coordinates::setW(float w){
    c[3] = w;
}

void Coordinates::setAll(float x, float y, float z){
    c[0] = x;
    c[1] = y;
    c[2] = z;
}

Coordinates Coordinates::baseChange(Coordinates o, Coordinates u, Coordinates v, Coordinates w){

}