#include "Ray.hpp"


void Ray::setDirection(Direction dir){
    this->dir = dir;
}

void Ray::setOrigin(Point origin){
    this->origin = origin;
}


int Ray::findIntersectionWith(ShapePtr shape, Point solutions[]){
    return shape->findIntersectionWithLine(dir, origin, solutions);
}


RGB Ray::getRayResult(Scene& scene){
    RGB result;
    Point solutions[2];
    double dist;

    int nShapes = scene.length();
    int intersections = 0;

    double minDistance = MAX_DOUBLE;

    ShapePtr closestShape = nullptr;
    ShapePtr shape;

    for(int s = 0; s < nShapes; s++){ //Iterate through all shapes in scene
        shape = scene.getShape(s);
        intersections = findIntersectionWith(shape, solutions); //Find intersection with ray
                    
        for(int i = 0; i < intersections; i++){   //Loop through intersections
            dist = distance(solutions[i], origin);    //Get distance to origin

            if(minDistance > dist && solutions[i].getZ() >= origin.getZ()){    //If distance is the smallest found AND object is not behind the camera
                minDistance = dist;  //Update minimum distance
                closestShape = shape;    //Update closest shape to camera
            }
        }
    }

    if(closestShape != nullptr){    //If any intersection was found with ray
        result = closestShape->getEmission(); //Set ray's color to the closest object's emission
    }

    return result;
}