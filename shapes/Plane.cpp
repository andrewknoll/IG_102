#include "Plane.hpp"


Plane::Plane(Direction normal, int distance){
    //nx*px + ny*py + nz*pz + d   0
    this->normal=normal;
    this->distanceToOrigin=distance;
}

void Plane::setAll(float coord[3], int distance){
    normal.setAll(coord[0], coord[1], coord[2]);
    this->distanceToOrigin = distance;
}

void Plane::calculateTangentsAtPoint(Point p, Direction& t1, Direction& t2){
    do{
    t1 = getRandomPoint() - p;
    } while(t1[0]==0 && t1[1]==0 && t1[2]==0);
    t1.normalize();
    t2 = cross(t1, normal);
    t2.normalize();
}

/*returns a point that meets these conditions:
    |Normal      | Output             |
    | nx  ny  nz |     x      y      z|
    ===================================
    |  0   0  !0 |   rnd    rnd  -d/nz|
    |  0  !0   0 |   rnd  -d/ny    rnd|
    |  0  !0  !0 |   rnd      0  -d/nz|
    | !0   0   0 | -d/nx    rnd    rnd|
    | !0   0  !0 |     0    rnd  -d/nz|
    | !0  !0   0 |     0  -d/ny    rnd|
    | !0  !0  !0 |     0      0  -d/nz|
 */
Point Plane::getRandomPoint(){
    Point p;
    bool assigned = false;
    for(int i =2; i >= 0; i--){
        if(!assigned && normal[i] != 0){
            p[i] = -distanceToOrigin / normal[i];
            assigned = true;
        }
        else if(normal[i] != 0){ //assigned == true
            p[i] = 0;
        }
        else{ //normal[i] == 0
            p[i] = rng.getNumber(0, 1);
        }
    }
    return p;
}

Direction Plane::getNormalAtPoint(Point p){
    return normal;
}

int Plane::findIntersectionWithLine(Direction d, Point o, double result[]){
    int solutions = 1;
    float num = -(distanceToOrigin + (o*normal));
    float denom = d * normal;
    if(denom == 0 && num != 0){
        solutions = 0;
    }
    else if(denom == 0){
        result[0] = 0;
    }
    else{
        result[0] = num/denom;
    }
    return solutions;
}