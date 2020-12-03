#include "Sphere.hpp"


Sphere::Sphere(Direction axis, Point center){
    this->axis = axis;
    this->center = center;
}

void Sphere::setCoords(float data[6]){
    axis.setAll(data[0], data[1], data[2]);
    center.setAll(data[3], data[4], data[5]);
}

Direction Sphere::getNormalAtPoint(Point p){
    Direction d = (p - center);
    d.normalize();
    return d;
}

void Sphere::calculateTangentsAtPoint(Point p, Direction& t1, Direction& t2){
    Direction normal = getNormalAtPoint(p);
    t1 = cross(normal, axis);
    t1.normalize();
    t2 = cross(t1, normal);
    t2.normalize();
}

int Sphere::findIntersectionWithLine(Direction d, Point o, double result[]){
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
        result[0] = roots[0];
        result[1] = roots[1];
        if(roots[0] != roots[1]){
            intersections = 2;
        }
        else{
            intersections = 1;
        }
    }

    return intersections;
}