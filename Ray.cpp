#include "Ray.hpp"

#define EPSILON 0.001

Direction Ray::getDirection(){
    return this->dir;
}

void Ray::setDirection(Direction dir){
    this->dir = dir;
}

void Ray::setOrigin(Point origin){
    this->origin = origin;
}


int Ray::findIntersectionWith(ShapePtr shape, float solutions[]){
    return shape->findIntersectionWithLine(dir, origin, solutions);
}

Event Ray::getShadowRayResult(Scene& scene, RGB& light, LightPoint destination, ShapePtr lastShape){
    ShapePtr closestShape = nullptr;
    ShapePtr shape;
    float solutions[2];

    int nShapes = scene.shapeN();
    bool foundObstacle = false;
    int nIntersections;
    float dist = distance(origin, destination.getLocalization());
    
    for(int s = 0; s < nShapes && !foundObstacle; s++){ //Iterate through all shapes in scene
        shape = scene.getShape(s);
        nIntersections = findIntersectionWith(shape, solutions); //Find intersection with ray
                    
        for(int i = 0; i < nIntersections && !foundObstacle; i++){   //Loop through intersections

            //foundObstacle = there was something in a distance smaller or equal to the distance to the destination,
            //and larger than 0, and the collision wasn't produced with the same shape it originated
            foundObstacle |= (dist >= solutions[i] && solutions[i] >= EPSILON);
        }
    }
    if(!foundObstacle){
        light = light + destination.getEmission() * max(lastShape->getNormalAtPoint(this->origin) * this->dir, 0.0f) * lastShape->getMaterial().getCoefficient(DIFFUSION) / (dist * dist * M_PI);
    }
    return foundObstacle ? NO_EVENT : LIGHTFOUND;
}

Event Ray::castShadowRays(Scene& scene, RGB& light, Point origin, ShapePtr lastShape){
    Ray shadowRay;
    shadowRay.setOrigin(origin);
    Direction dir;
    Point dest;
    Event e = NO_EVENT;

    for(int i = 0; i < scene.lightPN(); i++){
        dir = scene.getLightPoint(i).getLocalization() - origin;
        dir.normalize();
        shadowRay.setDirection(dir);
        e = shadowRay.getShadowRayResult(scene, light, scene.getLightPoint(i), lastShape);
    }
    return e;
}


RGB Ray::getRayResult(Scene& scene){
    RGB result= RGB(0,0,0), factor = RGB(1,1,1), indirectLight= RGB(0,0,0), directLight = RGB(0,0,0), directBounce= RGB(0,0,0), preFactor= RGB(1,1,1);
    float solutions[2];
    int nShapes = scene.shapeN();
    int nAreaLights = scene.areaLightN();
    int nIntersections = 0;

    float minT;
    bool foundAreaLight = false;

    ShapePtr closestShape;
    ShapePtr shape;
    ShapePtr lastShape = nullptr;

    AreaLight area;

    Material material;
    Direction newDirection, t1, t2, normal;
    Point intersection;
    Direction up = scene.getCamera().getUp();
    Direction left = scene.getCamera().getLeft();

    bool initialized = false;
    Event lastEvent = NO_EVENT, shadowEvent = NO_EVENT;

    do{
        preFactor = factor;
        closestShape = nullptr;
        minT = MAX_float;
        for(int s = 0; s < nShapes; s++){ //Iterate through all shapes in scene
            shape = scene.getShape(s);
            nIntersections = findIntersectionWith(shape, solutions); //Find intersection with ray
                        
            for(int i = 0; i < nIntersections; i++){   //Loop through intersections

                //If t is the smallest found AND object is not behind the origin
                //AND the shape hasn't produced a reflection on the ray
                if(minT > solutions[i] && solutions[i] >= EPSILON){
                    minT = solutions[i];  //Update minimum distance
                    closestShape = shape;    //Update closest shape
                }
            }
        }
        for(int al = 0; al < nAreaLights; al++){
            area = scene.getAreaLight(al);
            nIntersections = area.findIntersectionWithLine(dir, origin, solutions);
            if(nIntersections > 0){
                if(minT > solutions[0]){
                    minT = solutions[0];
                    closestShape = nullptr;
                    lastEvent = LIGHTFOUND;
                    foundAreaLight = true;
                }
            }
        }

        if(foundAreaLight){
            indirectLight = area.getEmission();
            lastEvent = LIGHTFOUND;
        } 
        else if(closestShape != nullptr){    //If any intersection was found with ray
            lastShape = closestShape;
            material = closestShape->getMaterial();
            intersection = minT * dir + origin;
            normal = closestShape->getNormalAtPoint(intersection);

            t1 = cross(normal, up);
            if(t1.isNull()){
                t1 = cross(normal, left);
            }
            t1.normalize();
            t2 = cross(normal, t1);
            t2.normalize();
            lastEvent = material.calculateRayCollision(factor,
                                                      indirectLight,
                                                      dir,
                                                      newDirection,
                                                      intersection,
                                                      normal,
                                                      t1,
                                                      t2,
                                                      initialized);
            setDirection(newDirection);
            setOrigin(intersection);

            if(lastEvent == DIFFUSION){
                shadowEvent = castShadowRays(scene, directBounce, intersection, lastShape);
                if(initialized){
                    directLight = directLight + preFactor * directBounce;
                }
                else{
                    directLight = directLight + directBounce;
                }
            }
            
        }
    }while(closestShape != nullptr && lastEvent != ABSORPTION && lastEvent != LIGHTFOUND);
    //bounce when we have found something to bounce on, and neither a light source was found nor the ray was absorbed

    if(lastEvent == LIGHTFOUND){
        if(initialized){
            result = factor * indirectLight + directLight;
        }
        else{
            result = indirectLight + directLight;
        }
    }
    else{
        result = directLight;
    }
    
    return result;
}