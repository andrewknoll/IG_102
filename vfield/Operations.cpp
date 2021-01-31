#include "Operations.hpp"

//***********************************************************************
// Unary -
// (Points)
//***********************************************************************
Point operator-(Point& p){
    return Point(-p['x'], -p['y'], -p['z']);
}

//***********************************************************************
// Unary -
// (Directions)
//***********************************************************************
Direction operator-(Direction& d){
    return Direction(-d['x'], -d['y'], -d['z']);
}

//***********************************************************************
// Point + Direction
//***********************************************************************
Point operator+(const Point p, const Direction d){
    return Point(p['x'] + d['x'], p['y'] + d['y'], p['z'] + d['z']);
}

//***********************************************************************
// Direction + Point
//***********************************************************************
Point operator+(const Direction d, const Point p){
    return p + d;
}

//***********************************************************************
// Point - Point
//***********************************************************************
Direction operator-(Point const &p1, Point const &p2){
    return Direction(p1['x'] - p2['x'], p1['y'] - p2['y'], p1['z'] - p2['z']);
}

//***********************************************************************
// Point - Direction
//***********************************************************************
Point operator-(Point const &p, Direction const &d){
    return Point(p['x'] - d['x'], p['y'] - d['y'], p['z'] - d['z']);
}

//***********************************************************************
// Direction + Direction
//***********************************************************************
Direction operator+(Direction const &d1, Direction const &d2){
    return Direction(d1['x'] + d2['x'], d1['y'] + d2['y'], d1['z'] + d2['z']);
}

//***********************************************************************
// Direction - Direction
//***********************************************************************
Direction operator-(Direction const &d1, Direction const &d2){
    return Direction(d1['x'] - d2['x'], d1['y'] - d2['y'], d1['z'] - d2['z']);
}

//***********************************************************************
// Direction * scalar
//***********************************************************************
Direction operator*(Direction const &d, const float s){
    return Direction(d['x'] * s, d['y'] * s, d['z'] * s);
}

//***********************************************************************
// scalar * Direction
//***********************************************************************
Direction operator* (const float s, Direction const &d){
    return d * s;
}

//***********************************************************************
// Direction / scalar
//***********************************************************************
Direction operator/(Direction const &d, const float s){
    return Direction(d['x'] / s, d['y'] / s, d['z'] / s);
}


//***********************************************************************
// Direction · Direction (dot product)
//***********************************************************************
float operator* (Direction const &d1, Direction const &d2){
    return d1['x'] * d2['x'] + d1['y'] * d2['y'] + d1['z'] * d2['z'];
}

//***********************************************************************
// Direction x Direction (cross product)
//***********************************************************************
Direction cross(Direction const &d1, Direction const &d2){
    return Direction(d1['y'] * d2['z'] - d1['z'] * d2['y'], d1['z'] * d2['x'] - d1['x'] * d2['z'], d1['x'] * d2['y'] - d1['y'] * d2['x']);
}

//***********************************************************************
//Matrix * Coordinates operator
//***********************************************************************
Coordinates operator* (Matrix4 m, Coordinates coord){
    Coordinates res;

    for(int i = 0; i < 4; i++){
        res[i] = 0.0;
        for(int j = 0; j < 4; j++){
            res[i] += m.get(i, j) * coord[j];
        }
    }
    return res;
}

//***********************************************************************
// Performs a translation transform
// @param coord Coordinates on which the transform will be performed
// @param tx Transform on axis X
// @param ty Transform on axis Y
// @param tz Transform on axis Z
// @returns Translated Coordinates
//***********************************************************************
Coordinates translate(Coordinates& coord, const float tx, const float ty, const float tz){
    Matrix4 mat;
    mat.identity();
    mat.set(0, 3, tx);
    mat.set(1, 3, ty);
    mat.set(2, 3, tz);
    mat.set(3, 3, 0.0);

    return mat * coord;
}

//***********************************************************************
// Performs a scale transform
// @param coord Coordinates on which the transform will be performed
// @param sx Transform on axis X
// @param sy Transform on axis Y
// @param sz Transform on axis Z
// @returns Scaled Coordinates
//***********************************************************************
Coordinates scale(Coordinates& coord, const float sx, const float sy, const float sz){
    Matrix4 mat;
    mat.set(0, 0, sx);
    mat.set(1, 1, sy);
    mat.set(2, 2, sz);
    mat.set(3, 3, 1.0);

    return mat * coord;
}

//***********************************************************************
// Performs a rotation transform
// @param coord Coordinates on which the transform will be performed
// @param axis Axis on which the transform will be performed
//      axis = 'x' Axis x
//      axis = 'y' Axis y
//      axis = 'z' Axis z
// @param angle Angle of rotation
// @returns Rotated Coordinates
//***********************************************************************
Coordinates rotate(Coordinates& coord, const char axis, const float angle){
    char aux = tolower(axis);
    assert(aux=='x' || aux=='y' || aux=='z');
    Matrix4 mat;
    mat.identity();
    switch(axis){
        case 'x':
            mat.set(1, 1, cos(angle));
            mat.set(1, 2, -sin(angle));
            mat.set(2, 1, sin(angle));
            mat.set(2, 2, cos(angle));
            break;
        case 'y':
            mat.set(0, 0, cos(angle));
            mat.set(2, 0, -sin(angle));
            mat.set(0, 2, sin(angle));
            mat.set(2, 2, cos(angle));
            break;
        case 'z':
            mat.set(0, 0, cos(angle));
            mat.set(0, 1, -sin(angle));
            mat.set(1, 0, sin(angle));
            mat.set(1, 1, cos(angle));
            break;
    }

    return mat * coord;
}

//***********************************************************************
// Performs a base change transform
// @tparam T type of the coordinates (Coordinates, Direction or Point)
// @param coord Coordinates to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The coordinates in the new base
//***********************************************************************
template <class T>
T baseChangeGeneric(T& coord, const Direction u, const Direction v, const Direction w, const Point o){
    Matrix4 mat;
    mat.setColumn(0, u.getVector());
    mat.setColumn(1, v.getVector());
    mat.setColumn(2, w.getVector());
    mat.setColumn(3, o.getVector());

    return mat * coord;
}

//***********************************************************************
// Performs a base change transform on a point
// @param coord Point to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The point in the new base
//***********************************************************************
Point baseChange(Point& coord, const Direction u, const Direction v, const Direction w, const Point o){
    Point result = baseChangeGeneric(coord, u, v, w, o);
    return result;
}

//***********************************************************************
// Performs a base change transform on a direction
// @param coord Direction to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The direction in the new base
//***********************************************************************
Direction baseChange(Direction& coord, const Direction u, const Direction v, const Direction w, const Point o){
    Direction result = baseChangeGeneric(coord, u, v, w, o);
    return result;
}

//***********************************************************************
// Performs a base change transform on some coordinates
// @param coord Coordinates to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The coordinates in the new base
//***********************************************************************
Coordinates baseChange(Coordinates& coord, const Direction u, const Direction v, const Direction w, const Point o){
    return baseChangeGeneric(coord, u, v, w, o);
}

//***********************************************************************
// Performs a base change transform, using the inverse of the matrix providen
// @tparam T type of the coordinates (Coordinates, Direction or Point)
// @param coord Coordinates to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The coordinates in the new base
//***********************************************************************
template <class T>
T baseChangeGenericInverse(T& coord, const Direction u, const Direction v, const Direction w, const Point o){
    Matrix4 mat;
    mat.setColumn(0, u.getVector());
    mat.setColumn(1, v.getVector());
    mat.setColumn(2, w.getVector());
    mat.setColumn(3, o.getVector());

    Matrix4 inv = mat.inverse();

    return inv * coord;
}

//***********************************************************************
// Performs a base change transform on a direction, using the inverse of the matrix providen
// @param coord Direction to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The direction in the new base
//***********************************************************************
Point baseChangeInverse(Point& coord, const Direction u, const Direction v, const Direction w, const Point o){
    Point result = baseChangeGenericInverse(coord, u, v, w, o);
    return result;
}

//***********************************************************************
// Performs a base change transform on a direction, using the inverse of the matrix providen
// @param coord Direction to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The direction in the new base
//***********************************************************************
Direction baseChangeInverse(Direction& coord, const Direction u, const Direction v, const Direction w, const Point o){
    Direction result = baseChangeGenericInverse(coord, u, v, w, o);
    return result;
}

//***********************************************************************
// Performs a base change transform on some coordinates, using the inverse of the matrix providen
// @param coord Coordinates to be transformed
// @param u A direction of the base.
// @param v A direction of the base. Must be perpendicular to u.
// @param w A direction of the base. Must be perpendicular to u and v.
// @param o Origin of the new base.
// @returns The coordinates in the new base
//***********************************************************************
Coordinates baseChangeInverse(Coordinates& coord, const Direction u, const Direction v, const Direction w, const Point o){
    return baseChangeGenericInverse(coord, u, v, w, o);
}

//***********************************************************************
// Solves a second degree equation of type Ax^2 + Bx + C = 0
// @param a Coefficient A
// @param b Coefficient B
// @param c Free term
// @param results[2] Array where the real roots will be stored (if any)
// @returns true if there are any real roots
//***********************************************************************
bool solveQuadraticEquation(float a, float b, float c, float results[2]){
    float insideSquareRoot = (b*b) - (4*a*c);
    bool isReal = (insideSquareRoot >= 0);
    if(isReal){
        results[0] = (-b + sqrt(insideSquareRoot))/(2*a);
        results[1] = (-b - sqrt(insideSquareRoot))/(2*a);
    }

    return isReal;
}

//***********************************************************************
// @param a Any point
// @param b Any point
// @returns Distance between a and b
//***********************************************************************
float distance(Point a, Point b){
    return (b - a).modulus();
}

//***********************************************************************
// Samples the angles compounding a solid angle in a hemisphere with Cosine Sampling
// @param inclination Will store the sampled inclination
// @param azimuth Will store the sampled azimuth
//***********************************************************************
void getAnglesByCosineSampling(float& inclination, float& azimuth){
    inclination = acosf(sqrt(1 - rng.getNumber(0,1)));
    azimuth = 2.0 * M_PI * rng.getNumber(0,1);
}

//***********************************************************************
// @param a Any direction
// @param b Any direction
// @returns Angle between a and b
//***********************************************************************
float findAngle(Direction a, Direction b){
    return acos((a * b)/(a.modulus() * b.modulus()));
}

//***********************************************************************
// Calculates the intersection of a plane, defined by it's normal and a point,
// and a line defined by a point and a direction
// @param normal Plane's normal
// @param origin A point in the plane
// @param lineDir Line's direction
// @param lineOrigin Line's origin
// @returns The factor of the intersection
//***********************************************************************
float solvePlaneLineIntersectionFromNormalAndPoint(Direction normal, Point origin, Direction lineDir, Point lineOrigin){
    float D = -(normal['x'] * origin['x'] + normal['y'] * origin['y'] + normal['z'] * origin['z']);

    float num = 0;
    float denum = 0;
    for(int i = 0; i < 3; i++){
        num += -normal[i]*lineOrigin[i];
        denum += normal[i]*lineDir[i];
    }
    return (num - D)/denum;
}