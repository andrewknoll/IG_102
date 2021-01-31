#include "Plane.hpp"

//***********************************************************************
// Class constructor.
// @param normal Surface normal
// @param distance Distance to world's origin (in equation Ax + By + Cz + D = 0, distance = D)
//***********************************************************************
Plane::Plane(Direction normal, int distance){
    //nx*px + ny*py + nz*pz + d   0
    this->myID = ++nextID;
    this->distanceToOrigin=distance;
    this->normal=normal;
    this->normal.normalize();
    
}

//***********************************************************************
// Sets all components from an array and an integer value
// @param coord[3] Array with the surface normal coordinates
// @param distance Distance to world's origin (in equation Ax + By + Cz + D = 0, distance = D)
//***********************************************************************
void Plane::setAll(float coord[3], int distance){
    normal.setAll(coord[0], coord[1], coord[2]);
    this->distanceToOrigin = distance;
}

//***********************************************************************
// Returns normal at the point given
// @param p Any point
// @returns Plane's surface's normal at point p
//***********************************************************************
Direction Plane::getNormalAtPoint(Point p){
    return normal;
}

//***********************************************************************
// Finds the intersection of a line with this plane
// @param d Direction of the line
// @param o Origin of the line
// @param result[] Array where the intersection factors will be stored
// @returns Number of intersection of the line and plane
//***********************************************************************
int Plane::findIntersectionWithLine(Direction d, Point o, float result[]){
    int solutions = 1;
    float num = distanceToOrigin + o*normal;
    float denom = d * normal;
    if(denom == 0 && num != 0){
        solutions = 0;
    }
    else if(denom == 0){    //Must return the minimum value that the path tracer will interpret as a collision
        result[0] = EPSILON;
    }
    else{
        result[0] = -num/denom;
    }
    return solutions;
}