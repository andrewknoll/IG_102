#include "Sphere.hpp"


Sphere::Sphere(Direction axis, Point center, RGB emission){
    this->axis = axis;
    this->center = center;
    this->emission = emission;
}

void Sphere::setCoords(float data[6]){
    axis.setAll(data[0], data[1], data[2]);
    center.setAll(data[3], data[4], data[5]);
    emission.set(data[0], 0);
    emission.set(data[1], 1);
    emission.set(data[2], 2);
}

int Sphere::findIntersectionWithLine(Direction d, Point o, Point result[]){
    double roots[2];
    double a, b, c, r;
    Point aux;
    bool isReal;
    int intersections = 0;

    a = d.modulus() * d.modulus();
    b = 2 * d * (o - center);
    aux = (o - center);
    r = (axis).modulus() / 2;
    c = aux * aux - r * r;

    isReal = solveQuadraticEquation(a, b, c, roots);
    if(isReal){
        result[0] = roots[0] * d;
        result[1] = roots[1] * d;
        if(roots[0] != roots[1]){
            intersections = 2;
        }
        else{
            intersections = 1;
        }
    }

    return intersections;
}