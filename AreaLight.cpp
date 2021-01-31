#include "AreaLight.hpp"

//***********************************************************************
// Default class constructor
//***********************************************************************
AreaLight::AreaLight(){
}

//***********************************************************************
// Class constructor
// @param origin Central point of area light
// @param base Vector of the base of the rectangle that forms the area light
// @param height Vector of the height of the rectangle that forms the area light
// @param boundU Length of base
// @param boundV Length of height
// @param emission RGB tuple of the light's emission
//***********************************************************************
AreaLight::AreaLight(Point origin, Direction base, Direction height, float boundU, float boundV, RGB emission){
    this->emission = emission;
    this->base = base;
    this->height = height;
    this->origin = origin;
    this->boundU = boundU;
    this->boundV = boundV;
}

//***********************************************************************
// Sets the value of the emission
// @param emission New RGB tuple of the light's emission
//***********************************************************************
void AreaLight::setEmission(RGB emission){
    this->emission = emission;
}

//***********************************************************************
// Returns light's emission
// @returns RGB tuple of the light's emission
//***********************************************************************
RGB AreaLight::getEmission(){
    return emission;
}

//***********************************************************************
// Sets the origin
// @param origin New origin for the area light
//***********************************************************************
void AreaLight::setOrigin(Point origin){
    this->origin = origin;
}

//***********************************************************************
// Returns the origin
// @returns Center point of the area light
//***********************************************************************
Point AreaLight::getOrigin(){
    return origin;
}

//***********************************************************************
// Sets the base vector
// @param base New base for the area light
//***********************************************************************
void AreaLight::setBase(Direction base){
    this->base = base;
}

//***********************************************************************
// Returns the base vector
// @returns Base vector of the area light
//***********************************************************************
Direction AreaLight::getBase(){
    return base;
}

//***********************************************************************
// Sets the height vector
// @param height New height for the area light
//***********************************************************************
void AreaLight::setHeight(Direction height){
    this->height = height;
}

//***********************************************************************
// Returns the height vector
// @returns Height vector of the area light
//***********************************************************************
Direction AreaLight::getHeight(){
    return height;
}

//***********************************************************************
// Finds the intersection of a line with the rectangle of the area light
// @param d Direction of the line
// @param o Origin of the line
// @param result[] Array where the intersection factors will be stored
// @returns Number of intersection of the line and sphere
//***********************************************************************
int AreaLight::findIntersectionWithLine(Direction d, Point o, float result[]){
    
    int solutions = 0;
    Direction normal = cross(base, height);

    //Calculate intersection with the plane that contains the rectangle
    float t = solvePlaneLineIntersectionFromNormalAndPoint(normal, origin, d, o);
    Point intersection = o + t * d;

    int i = 0;
    int j = 0;
    
    //Let i,j for: i != j
    //and base[i] != 0
    //and height[j] != 0
    while(i < 3 && base[i] == 0){
        i++;
    }
    while(j < 3 && (height[j] == 0 || i == j)){
        j++;
    }

    //Check bounds for the rectangle
    float v = (intersection[j]*base[i] - origin[j]*base[i] + base[j]*origin[j] - base[j]*intersection[i])/(base[i]*height[j] - base[j]*height[i]);
    float u = (intersection[i] - origin[i] - v*height[i])/base[i];

    if(abs(u) <= boundU && abs(v) <= boundV){ //if it is inside bounds
        result[0] = t;
        solutions = 1;
    }

    return solutions;
}
