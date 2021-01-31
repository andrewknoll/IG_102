#include "Coordinates.hpp"

//***********************************************************************
// Class default constructor
// Sets all values to 0
//***********************************************************************
Coordinates::Coordinates(){
    c[0] = 0.0;
    c[1] = 0.0;
    c[2] = 0.0;
    c[3] = 0.0;
}

//***********************************************************************
// Class constructor.
// @param x X coordinate
// @param y Y coordinate
// @param z Z coordinate
// @param w Homogenous coordinate
//***********************************************************************
Coordinates::Coordinates(float x, float y, float z, float w){
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
}

//***********************************************************************
// @returns X component
//***********************************************************************
float Coordinates::getX() const{
    return c[0];
}

//***********************************************************************
// @returns Y component
//***********************************************************************
float Coordinates::getY() const{
    return c[1];
}

//***********************************************************************
// @returns Z component
//***********************************************************************
float Coordinates::getZ() const{
    return c[2];
}

//***********************************************************************
// @returns Homogenous coordinate
//***********************************************************************
float Coordinates::getW() const{
    return c[3];
}

//***********************************************************************
// Sets X's value
// @param x New X value
//***********************************************************************
void Coordinates::setX(float x){
    c[0] = x;
}

//***********************************************************************
// Sets Y's value
// @param y New Y value
//***********************************************************************
void Coordinates::setY(float y){
    c[1] = y;
}

//***********************************************************************
// Sets Z's value
// @param z New Z value
//***********************************************************************
void Coordinates::setZ(float z){
    c[2] = z;
}

//***********************************************************************
// Sets Homogenous coordinate's value
// @param w New homogenous coordinate value
//***********************************************************************
void Coordinates::setW(float w){
    c[3] = w;
}

//***********************************************************************
// Sets x, y and z values
// @param x New X value
// @param y New Y value
// @param z New Z value
//***********************************************************************
void Coordinates::setAll(float x, float y, float z){
    c[0] = x;
    c[1] = y;
    c[2] = z;
}

//***********************************************************************
// @returns a pointer to an array with the 4 coordinates
//***********************************************************************
const float* Coordinates::getVector() const{
    return c;    
}

//***********************************************************************
// Returns a reference to the i'th coordinate
// @param i Coordinate to be referenced
// @returns a reference to the i'th coordinate
//***********************************************************************
float& Coordinates::operator[](int i){
    return c[i];
}

//***********************************************************************
// Returns the value of the i'th coordinate
// @param i Coordinate to be returned
// @returns the value of the i'th coordinate
//***********************************************************************
float Coordinates::operator[](int i) const {
    return c[i];
}

//***********************************************************************
// Returns a reference to the coordinate indicated by a character
// @param a Character that referenciates what coordinate is going to be returned.
//      if a == 'x', coordinate X will be returned
//      if a == 'y', coordinate Y will be returned
//      if a == 'z', coordinate Z will be returned
//      if a == 'w', coordinate W will be returned
// @throws asssertionException if a isn't any of the possible values
// @returns a reference to the coordinate
//***********************************************************************
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

//***********************************************************************
// Returns the value of the coordinate indicated by a character
// @param a Character that referenciates what coordinate is going to be returned.
//      if a == 'x', coordinate X will be returned
//      if a == 'y', coordinate Y will be returned
//      if a == 'z', coordinate Z will be returned
//      if a == 'w', coordinate W will be returned
// @throws asssertionException if a isn't any of the possible values
// @returns the value of the coordinate
//***********************************************************************
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

//***********************************************************************
// Checks if the values of x, y and z are equal to 0
// @returns true if and only if all x, y and z are equal to 0
//***********************************************************************
bool Coordinates::isNull(){
    return c[0] == 0 && c[1] == 0 && c[2] == 0;
}

//***********************************************************************
// * Unused
// @param m A 4x4 matrix
// @returns the product of these coordinates * m
//***********************************************************************
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

//***********************************************************************
// Prints on the specified output stream the coordinates in format (X, Y, Z, W)
// @param os An output stream
// @param coord Some coordinates
// @relatealso Coordinates
// @returns A reference to ostram os (allows for concatenation of operations)
//***********************************************************************
ostream& operator<<(ostream& os, const Coordinates& coord)
{
    os << "(" << coord.getX() << ", " << coord.getY() << ", " << coord.getZ() << ", " << coord.getW() << ")";
    return os;
}