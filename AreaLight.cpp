#include "AreaLight.hpp"

AreaLight::AreaLight(){
}

AreaLight::AreaLight(Point origin, Direction base, Direction height, float boundU, float boundV, RGB emission){
    this->emission = emission;
    this->base = base;
    this->height = height;
    this->origin = origin;
    this->boundU = boundU;
    this->boundV = boundV;
}

void AreaLight::setEmission(RGB emission){
    this->emission = emission;
}

RGB AreaLight::getEmission(){
    return emission;
}

void AreaLight::setOrigin(Point origin){
    this->origin = origin;
}

Point AreaLight::getOrigin(){
    return origin;
}

void AreaLight::setBase(Direction base){
    this->base = base;
}

Direction AreaLight::getBase(){
    return base;
}

void AreaLight::setHeight(Direction height){
    this->height = height;
}

Direction AreaLight::getHeight(){
    return height;
}

int AreaLight::findIntersectionWithLine(Direction d, Point o, double result[]){
    int solutions = 0;
    Direction normal = cross(base, height);

    float t = solvePlaneLineIntersectionFromNormalAndPoint(normal, origin, d, o);
    Point intersection = o + t * d;


    int i = 0;
    int j = 0;
    
    //i != j != k
    //base[i] != 0
    //height[j] != 0
    while(i < 3 && base[i] == 0){
        i++;
    }
    while(j < 3 && (height[j] == 0 || i == j)){
        j++;
    }
    int k = 3 -i -j;

    float v = (intersection[j]*base[i] - origin[j]*base[i] - intersection[i] + origin[i])/(base[i]*height[j] - height[i]);
    float u = (intersection[i] - origin[i] - v*height[i])/base[i];

    if((intersection[k] - origin[k] + u*base[k] + v*height[k]) < 0.0001){
        if(abs(u) <= boundU && abs(v) <= boundV && t >=0){
            result[0] = t;
            solutions = 1;
        }
    }
    return solutions;
}
