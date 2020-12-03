#include "Ray.hpp"


Direction Ray::getDirection(){
    return this->dir;
}

void Ray::setDirection(Direction dir){
    this->dir = dir;
}

void Ray::setOrigin(Point origin){
    this->origin = origin;
}


int Ray::findIntersectionWith(ShapePtr shape, double solutions[]){
    return shape->findIntersectionWithLine(dir, origin, solutions);
}


RGB Ray::getRayResult(Scene& scene){
    RGB result(1,1,1);
    double solutions[2];
    int nShapes = scene.length();
    int nIntersections = 0;

    double minT = MAX_DOUBLE;

    ShapePtr closestShape = nullptr;
    ShapePtr shape;

    Material material;
    Direction newDirection, t1, t2;
    Point intersection;

    bool finished;

    do{
        finished = false;
        for(int s = 0; s < nShapes; s++){ //Iterate through all shapes in scene
            shape = scene.getShape(s);
            nIntersections = findIntersectionWith(shape, solutions); //Find intersection with ray
                        
            for(int i = 0; i < nIntersections; i++){   //Loop through intersections

                if(minT > solutions[i] && solutions[i] >= 0){    //If t is the smallest found AND object is not behind the camera
                    minT = solutions[i];  //Update minimum distance
                    closestShape = shape;    //Update closest shape to camera
                }
            }
        }

        if(closestShape != nullptr){    //If any intersection was found with ray
            material = closestShape->getMaterial();
            intersection = minT * dir + origin;
            closestShape->calculateTangentsAtPoint(intersection, t1, t2);

            finished = material.calculateRayCollision(result,
                                                      dir,
                                                      newDirection,
                                                      intersection,
                                                      closestShape->getNormalAtPoint(intersection),
                                                      t1,
                                                      t2);
            setOrigin(intersection);
        }
    }while(closestShape != nullptr && !finished);
     //bounce when we have found something to bounce on, and we haven't found a light yet
    return result;
}