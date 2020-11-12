#include "Sphere.hpp"

Sphere::Sphere(Direction axis, Point center, RGB emission){
    this->axis = axis;
    this->center = center;
    this->emission = emission;
}

void Sphere::setCoords(float data[6]){
    axis.setAll(data[0], data[1], data[2]);
    center.setAll(data[3], data[4], data[5]);
    emission.set(data[0]);
    emission.set(data[1]);
    emission.set(data[2]);
}