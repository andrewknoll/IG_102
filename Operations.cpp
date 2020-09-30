#include "Operations.h"

// Point + Direction
Point operator+(const Point p, const Direction d){
    return Point(p['x'] + d['x'], p['y'] + d['y'], p['z'] + d['z']);
}

// Direction + Point
Point operator+(const Direction d, const Point p){
    return p + d;
}

// Point - Point
Direction operator-(Point const &p1, Point const &p2){
    return Direction(p1['x'] - p2['x'], p1['y'] - p2['y'], p1['z'] - p2['z']);
}

// Direction + Direction
Direction operator-(Direction const &d1, Direction const &d2){
    return Direction(d1['x'] + d2['x'], d1['y'] + d2['y'], d1['z'] + d2['z']);
}

// Direction - Direction
Direction operator-(Direction const &d1, Direction const &d2){
    return Direction(d1['x'] - d2['x'], d1['y'] - d2['y'], d1['z'] - d2['z']);
}

// Direction * scalar
Direction operator*(Direction const &d, const float s){
    return Direction(d['x'] * s, d['y'] * s, d['z'] * s);
}

// scalar * Direction
Direction operator* (const float s, Direction const &d){
    return d * s;
}

// Direction / scalar
Direction operator/(Direction const &d, const float s){
    return Direction(d['x'] / s, d['y'] / s, d['z'] / s);
}


// Direction * Direction (dot)
float operator* (Direction const &d1, Direction const &d2){
    return d1['x'] * d2['x'] + d1['y'] * d2['y'] + d1['z'] * d2['z'];
}

// Direction x Direction (cross)
Direction cross(Direction const &d1, Direction const &d2){
    return Direction(d1['y'] * d2['z'] - d1['z'] * d2['y'], d1['z'] * d2['x'] - d1['x'] * d2['z'], d1['x'] * d2['y'] - d1['y'] * d2['x']);
}


Coordinates translate(Coordinates& coord, const float tx, const float ty, const float tz){
    Matrix4 mat;
    mat.identity();
    mat.set(0, 3, tx);
    mat.set(1, 3, ty);
    mat.set(2, 3, tz);
    mat.set(3, 3, 0.0);

    return coord * mat;
}

Coordinates scale(Coordinates& coord, const float sx, const float sy, const float sz){
    Matrix4 mat;
    mat.set(0, 0, sx);
    mat.set(1, 1, sy);
    mat.set(2, 2, sz);
    mat.set(3, 3, 1.0);

    return coord * mat;
}

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

    return coord * mat;
}

Coordinates baseChange(Coordinates& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Matrix4 mat;
    mat.setColumn(0, u.getVector());
    mat.setColumn(1, v.getVector());
    mat.setColumn(2, w.getVector());
    mat.setColumn(3, o.getVector());

    return coord * mat;
}