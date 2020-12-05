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
    RGB result;
    double solutions[2];
    int nShapes = scene.length();
    int nIntersections = 0;

    double minT;

    ShapePtr closestShape;
    ShapePtr shape;
    ShapePtr lastShape = nullptr;

    Material material;
    Direction newDirection, t1, t2;
    Point intersection;

    bool initialized = false;
    Event lastEvent = NO_EVENT;

    do{
        closestShape = nullptr;
        minT = MAX_DOUBLE;
        for(int s = 0; s < nShapes; s++){ //Iterate through all shapes in scene
            shape = scene.getShape(s);
            nIntersections = findIntersectionWith(shape, solutions); //Find intersection with ray
                        
            for(int i = 0; i < nIntersections; i++){   //Loop through intersections

                //If t is the smallest found AND object is not behind the camera
                //AND the shape hasn't produced a reflection on the ray
                if(minT > solutions[i] && solutions[i] > 0 
                && (!areEqual(lastShape, shape) || lastEvent == REFRACTION || lastShape == nullptr)){    
                    minT = solutions[i];  //Update minimum distance
                    closestShape = shape;    //Update closest shape to camera
                }
            }
        }

        if(closestShape != nullptr){    //If any intersection was found with ray
            lastShape = closestShape;
            material = closestShape->getMaterial();
            intersection = minT * dir + origin;
            closestShape->calculateTangentsAtPoint(intersection, t1, t2);
            lastEvent = material.calculateRayCollision(result,
                                                      dir,
                                                      newDirection,
                                                      intersection,
                                                      closestShape->getNormalAtPoint(intersection),
                                                      t1,
                                                      t2,
                                                      initialized);
            setDirection(newDirection);
            setOrigin(intersection);
        }
        else{
            result = RGB(0, 0, 0);
        }
    }while(closestShape != nullptr && lastEvent != ABSORTION && !material.is(LIGHTSOURCE));
    //bounce when we have found something to bounce on, and neither a light source was found nor the ray was absorbed
    return result;
}