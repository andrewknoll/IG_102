#include "Sphere.hpp"

//***********************************************************************
// Class constructor
// @param axis Axis of the sphere
// @param center Center of the sphere
//***********************************************************************
Sphere::Sphere(Direction axis, Point center){
    this->myID = ++nextID;
    this->axis = axis;
    this->center = center;
}

//***********************************************************************
// Sets all components from an array
// @param coord[6] Array with the axis and center coordinates
//***********************************************************************
void Sphere::setCoords(float data[6]){
    axis.setAll(data[0], data[1], data[2]);
    center.setAll(data[3], data[4], data[5]);
}

//***********************************************************************
// Returns normal at the point given
// @param p Any point of the sphere
// @returns Sphere's surface's normal at point p
//***********************************************************************
Direction Sphere::getNormalAtPoint(Point p){
    Direction d = (p - center);
    d.normalize();
    return d;
}

//***********************************************************************
// Finds the intersection of a line with this sphere
// @param d Direction of the line
// @param o Origin of the line
// @param result[] Array where the intersection factors will be stored
// @returns Number of intersection of the line and sphere
//***********************************************************************
int Sphere::findIntersectionWithLine(Direction d, Point o, float result[]){
    float roots[2];
    float a, b, c, r;
    float aux;
    bool isReal;
    int intersections = 0;

    a = d.modulus() * d.modulus();
    b = 2 * d * (o - center);
    aux = (o - center).modulus();
    r = (axis).modulus() / 2;
    c = aux * aux - r * r;

    isReal = solveQuadraticEquation(a, b, c, roots);
    if(isReal){ //Line touches / cuts sphere
        result[0] = roots[0];
        result[1] = roots[1];
        if(roots[0] != roots[1]){
            intersections = 2;
        }
        else{   //Line is tangent to sphere
            intersections = 1;
        }
    }

    return intersections;
}