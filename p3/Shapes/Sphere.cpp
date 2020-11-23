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

void Sphere::findFirstIntersectionWithLine(Direction d, Point o, struct PossibleSolution<Point> result[]){
    struct PossibleSolution<double> roots[2];
    double a = d.modulus() * d.modulus();
    double b = 2 * d * (o - center);
    Point aux = (o - center);
    double r = (axis).modulus() / 2;
    double c = aux * aux - r * r;
    solveQuadraticEquation(a, b, c, roots);
    if(roots[0].doesExist()){
        result[0].setSolution(roots[0].getSolution() * d);
        result[1].setSolution(roots[1].getSolution() * d);
    }
    else{
        result[0].setNotReal();
        result[1].setNotReal();
    }
}