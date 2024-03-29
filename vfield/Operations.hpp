#include "Point.hpp"
#include "Direction.hpp"
#include "../rng.hpp"
#include <math.h>

#ifndef __OPERATIONS__
#define __OPERATIONS__


//Unary -
Point operator-(Point& p);

//Unary -
Direction operator-(Direction& d);

// Point + Direction
Point operator+(const Point p, const Direction d);

// Direction + Point
Point operator+(const Direction d, const Point p);

// Point - Point
Direction operator-(Point const &p1, Point const &p2);

// Point - Direction
Point operator-(Point const &p, Direction const &d);

// Direction + Direction
Direction operator+(Direction const &d1, Direction const &d2);

// Direction - Direction
Direction operator-(Direction const &d1, Direction const &d2);

// Direction * scalar
Direction operator*(Direction const &d, const float s);

// scalar * Direction
Direction operator* (const float s, Direction const &d);

// Direction / scalar
Direction operator/(Direction const &d, const float s);


// Direction * Direction (dot)
float operator* (Direction const &d1, Direction const &d2);

// Direction x Direction (cross)
Direction cross(Direction const &d1, Direction const &d2);


Coordinates translate(Coordinates& coord, const float tx, const float ty, const float tz);
Coordinates scale(Coordinates& coord, const float sx, const float sy, const float sz);
Coordinates rotate(Coordinates& coord, const char axis, const float angle);

template <class T>
T baseChangeGeneric(T& coord, const Direction u, const Direction v, const Direction w, const Point o);
Point baseChange(Point& coord, const Direction u, const Direction v, const Direction w, const Point o);
Direction baseChange(Direction& coord, const Direction u, const Direction v, const Direction w, const Point o);
Coordinates baseChange(Coordinates& coord, const Direction u, const Direction v, const Direction w, const Point o);

template <class T>
T baseChangeGenericInverse(T& coord, const Direction u, const Direction v, const Direction w, const Point o);
Point baseChangeInverse(Point& coord, const Direction u, const Direction v, const Direction w, const Point o);
Direction baseChangeInverse(Direction& coord, const Direction u, const Direction v, const Direction w, const Point o);
Coordinates baseChangeInverse(Coordinates& coord, const Direction u, const Direction v, const Direction w, const Point o);

bool solveQuadraticEquation(float a, float b, float c, float results[2]);
float distance(Point a, Point b);

void getAnglesByCosineSampling(float& inclination, float& azimuth);
float findAngle(Direction a, Direction b);

float solvePlaneLineIntersectionFromNormalAndPoint(Direction normal, Point origin, Direction lineDir, Point lineOrigin);

#endif