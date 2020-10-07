#include "Point.hpp"
#include "Direction.hpp"
#include <math.h>

#ifndef OPERATIONS
#define OPERATIONS

// Point + Direction
Point operator+(const Point p, const Direction d);

// Direction + Point
Point operator+(const Direction d, const Point p);

// Point - Point
Direction operator-(Point const &p1, Point const &p2);

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
T baseChangeGeneric(T& coord, const Point o, const Direction u, const Direction v, const Direction w);
Point baseChange(Point& coord, const Point o, const Direction u, const Direction v, const Direction w);
Direction baseChange(Direction& coord, const Point o, const Direction u, const Direction v, const Direction w);
Coordinates baseChange(Coordinates& coord, const Point o, const Direction u, const Direction v, const Direction w);

#endif