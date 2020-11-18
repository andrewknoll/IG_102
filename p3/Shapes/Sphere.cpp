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

PossibleSolution<Point> Sphere::findFirstIntersectionWithLine(Direction d, Point o){
    PossibleSolution<double> roots[2];
    PossibleSolution<Point> result;
    double a = d.modulus() * d.modulus();
    double b = 2 * d * (o - center);
    double c = (o - center).modulus() - (axis / 2).modulus();
    solveQuadraticEquation(a, b, c, roots);
    if(roots[0].doesExist()){
        result.setSolution(roots[0].getSolution() * d);
    }
    return result;
}