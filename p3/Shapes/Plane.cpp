#include "Plane.hpp"

Plane::Plane(Direction normal, int distance, RGB emission){
    this->normal=normal;
    this->distanceToOrigin=distance;
    this->emission = emission;
}

void Plane::setAll(float coord[3], int distance, float emission[3]){
    normal.setAll(coord[0], coord[1], coord[2]);
    this->distanceToOrigin = distance;
    
    this->emission.set(emission[0], 0);
    this->emission.set(emission[1], 1);
    this->emission.set(emission[2], 2);
}

PossibleSolution<Point> Plane::findFirstIntersectionWithLine(Direction d, Point o){
    PossibleSolution<Point> result;
    float t = -(distanceToOrigin + (o * normal))/(d * normal);
    result.setSolution((d * t) + o);
    return result;
}