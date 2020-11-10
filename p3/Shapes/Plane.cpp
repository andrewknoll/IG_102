#include "Plane.hpp"

Plane::Plane(Direction normal, int distance, RGB emission){
    this->normal=normal;
    this->distanceToOrigin=distance;
    this->emission = emission;
}

void Plane::setAll(float coord[3], int distance, float emission[3]){
    normal.setAll(coord[0], coord[1], coord[2]);
    this->distanceToOrigin = distance;
    this->emission(emission[0], emission[1], emission[2]);
}