#include "Coordinates.hpp"

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

const float* Coordinates::getVector() const{
    return c;    
}

float& Coordinates::operator[](int i){
    return c[i];
}

float Coordinates::operator[](int i) const {
    return c[i];
}

float& Coordinates::operator[](char a){
    char i = tolower(a);
    switch(i){
        case 'x':
        return c[0];

        case 'y':
        return c[1];

        case 'z':
        return c[2];

        case 'w':
        return c[3];

        default:
        assert(i=='x' || i=='y' || i=='z' || i=='w');
    }
    return c[0];
}

float Coordinates::operator[](char a) const {
    char i = tolower(a);
    switch(i){
        case 'x':
        return c[0];

        case 'y':
        return c[1];

        case 'z':
        return c[2];

        case 'w':
        return c[3];

        default:
        assert(i=='x' || i=='y' || i=='z' || i=='w');
    }
    return 0;
}

//Coordinates * Matrix operator
Coordinates Coordinates::operator* (Matrix4 m) const{
    Coordinates res;

    for(int i = 0; i < 4; i++){
        res[i] = 0.0;
        for(int j = 0; j < 4; j++){
            res[i] += c[j] * m.get(j, i);
        }
    }
    return res;
}

ostream& operator<<(ostream& os, const Coordinates& coord)
{
    os << "(" << coord.getX() << ", " << coord.getY() << ", " << coord.getZ() << ", " << coord.getW() << ")";
    return os;
}