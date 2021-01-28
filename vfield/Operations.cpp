#include "Operations.hpp"

Point operator-(Point& p){
    return Point(-p['x'], -p['y'], -p['z']);
}

Direction operator-(Direction& d){
    return Direction(-d['x'], -d['y'], -d['z']);
}

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

// Point - Direction
Point operator-(Point const &p, Direction const &d){
    return Point(p['x'] - d['x'], p['y'] - d['y'], p['z'] - d['z']);
}

// Direction + Direction
Direction operator+(Direction const &d1, Direction const &d2){
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

//Matrix * Coordinates operator
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


Coordinates translate(Coordinates& coord, const float tx, const float ty, const float tz){
    Matrix4 mat;
    mat.identity();
    mat.set(0, 3, tx);
    mat.set(1, 3, ty);
    mat.set(2, 3, tz);
    mat.set(3, 3, 0.0);

    return mat * coord;
}

Coordinates scale(Coordinates& coord, const float sx, const float sy, const float sz){
    Matrix4 mat;
    mat.set(0, 0, sx);
    mat.set(1, 1, sy);
    mat.set(2, 2, sz);
    mat.set(3, 3, 1.0);

    return mat * coord;
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

    return mat * coord;
}
template <class T>
T baseChangeGeneric(T& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Matrix4 mat;
    mat.setColumn(0, u.getVector());
    mat.setColumn(1, v.getVector());
    mat.setColumn(2, w.getVector());
    mat.setColumn(3, o.getVector());

    return mat * coord;
}

Point baseChange(Point& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Point result = baseChangeGeneric(coord, o, u, v, w);
    return result;
}

Direction baseChange(Direction& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Direction result = baseChangeGeneric(coord, o, u, v, w);
    return result;
}

Coordinates baseChange(Coordinates& coord, const Point o, const Direction u, const Direction v, const Direction w){
    return baseChangeGeneric(coord, o, u, v, w);
}

template <class T>
T baseChangeGenericInverse(T& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Matrix4 mat;
    mat.setColumn(0, u.getVector());
    mat.setColumn(1, v.getVector());
    mat.setColumn(2, w.getVector());
    mat.setColumn(3, o.getVector());

    Matrix4 inv = mat.inverse();

    return inv * coord;
}

Point baseChangeInverse(Point& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Point result = baseChangeGenericInverse(coord, o, u, v, w);
    return result;
}

Direction baseChangeInverse(Direction& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Direction result = baseChangeGenericInverse(coord, o, u, v, w);
    return result;
}

Coordinates baseChangeInverse(Coordinates& coord, const Point o, const Direction u, const Direction v, const Direction w){
    return baseChangeGenericInverse(coord, o, u, v, w);
}

bool solveQuadraticEquation(float a, float b, float c, float results[2]){
    float insideSquareRoot = b*b - 4*a*c;
    bool isReal = (insideSquareRoot >= 0);
    if(isReal){
        results[0] = (-b + sqrt(insideSquareRoot))/(2*a);
        results[1] = (-b - sqrt(insideSquareRoot))/(2*a);
    }

    return isReal;
}

float distance(Point a, Point b){
    return (b - a).modulus();
}

void getAnglesByCosineSampling(float& inclination, float& azimuth){
    inclination = acosf(sqrt(1 - rng.getNumber(0,1)));
    azimuth = 2.0 * M_PI * rng.getNumber(0,1);
}

float findAngle(Direction a, Direction b){
    return acos((a * b)/(a.modulus() * b.modulus()));
}

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