#include "Ray.hpp"

//***********************************************************************
// @returns Ray's direction
//***********************************************************************
Direction Ray::getDirection(){
    return this->dir;
}

//***********************************************************************
// Sets the direction of this ray
// @param dir New direction for the ray
//***********************************************************************
void Ray::setDirection(Direction dir){
    this->dir = dir;
}

//***********************************************************************
// Sets the origin of this ray
// @param dir New origin for the ray
//***********************************************************************
void Ray::setOrigin(Point origin){
    this->origin = origin;
}

//***********************************************************************
// Finds the intersection of this ray with the shape
// @param shape Smart pointer to the shape
// @param solutions[] Array that will store the factor of the intersections (if any)
// @returns Number of intersections
//***********************************************************************
int Ray::findIntersectionWith(ShapePtr shape, float solutions[]){
    return shape->findIntersectionWithLine(dir, origin, solutions);
}

//***********************************************************************
// Calculates the direct contribution of a light point
// @param scene The scene the ray is being casted at
// @param light Will be modified with the contribution of the light point
// @param destination Light point whose contribution will be calculated
// @param lastShape Smart pointer with the shape the shadow ray is being casted from
// @returns false if and only if an obstacle was found.
//***********************************************************************
bool Ray::getShadowRayResult(Scene& scene, RGB& light, LightPoint destination, ShapePtr lastShape){
    ShapePtr closestShape = nullptr;
    ShapePtr shape;
    float solutions[2];

    int nShapes = scene.shapeN();
    //last shape's normal and this direction should be in the same direction (dot pruduct must be >0)
    bool foundObstacle = (lastShape->getNormalAtPoint(this->origin) * this->dir <= 0);
    int nIntersections;
    float dist = distance(origin, destination.getLocalization());
    float dist2;

    for(int s = 0; s < nShapes && !foundObstacle; s++){ //Iterate through all shapes in scene
        shape = scene.getShape(s);
        nIntersections = findIntersectionWith(shape, solutions); //Find intersection with ray
                    
        for(int i = 0; i < nIntersections && !foundObstacle; i++){   //Loop through intersections
            //safe check, in case direction is not normalized
            dist2 = (solutions[i] * dir).modulus();
            //foundObstacle = there was something in a distance smaller or equal to the distance to the destination,
            //and larger than 0, and the collision wasn't produced with the same shape it originated
            foundObstacle |= (dist >= dist2 && solutions[i] >= EPSILON);
        }
    }
    if(!foundObstacle){
        light = light + destination.getEmission() * (lastShape->getNormalAtPoint(this->origin) * this->dir) * lastShape->getMaterial().getCoefficient(DIFFUSION) / (dist * dist * M_PI);
    }

    return !foundObstacle;
}

//***********************************************************************
// Calculates the direct contribution of all light points
// @param scene The scene the ray is being casted at
// @param light Will be modified with the contribution of the light point
// @param origin Origin of the shadow rays
// @param lastShape Smart pointer with the shape the shadow ray is being casted from
// @returns NO_EVENT if no change was made. Otherwise, LIGHTFOUND
//***********************************************************************
Event Ray::castShadowRays(Scene& scene, RGB& light, Point origin, ShapePtr lastShape){
    Ray shadowRay;
    shadowRay.setOrigin(origin);
    Direction d;
    Point dest;
    bool e = false;

    for(int i = 0; i < scene.lightPN(); i++){
        d = scene.getLightPoint(i).getLocalization() - origin;
        d.normalize();
        shadowRay.setDirection(d);
        e |= shadowRay.getShadowRayResult(scene, light, scene.getLightPoint(i), lastShape);
    }
    return e? LIGHTFOUND : NO_EVENT;
}

//***********************************************************************
// Calculates the result of tracing a ray until it gets absorbed, finds a light or nothing was found
// @param scene The scene the ray is being casted at
// @returns The RGB tuple containing the color that was calculated
//***********************************************************************
RGB Ray::getRayResult(Scene& scene){
    //Variable initializations
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

    //Bounce
    do{
        preFactor = factor; //prefactor will always contain the throughput before the current bounce was added
        closestShape = nullptr;
        minT = MAX_float;

        //---------------------------------------------------------------------------------------------------
        //Calculate nearest intersection    
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
        //---------------------------------------------------------------------------------------------------
        //Calculate intersections with Area Lights
        for(int al = 0; al < nAreaLights; al++){
            area = scene.getAreaLight(al);
            nIntersections = area.findIntersectionWithLine(dir, origin, solutions);
            if(nIntersections > 0 ){
                if(minT > solutions[0] && solutions[0] >= EPSILON){
                    minT = solutions[0];
                    //Clear closest shape and set the variables to indicate we've found a light
                    closestShape = nullptr;
                    lastEvent = LIGHTFOUND;
                    foundAreaLight = true;
                }
            }
        }
        //---------------------------------------------------------------------------------------------------
        //Get intersection results
        if(foundAreaLight){
            indirectLight = area.getEmission();
            lastEvent = LIGHTFOUND;
        } 
        else if(closestShape != nullptr){    //If any intersection was found with ray
            lastShape = closestShape;
            material = closestShape->getMaterial();
            intersection = minT * dir + origin;
            normal = closestShape->getNormalAtPoint(intersection);

            //Calculate tangents
            t1 = cross(normal, up);
            if(t1.isNull()){
                t1 = cross(normal, left);
            }
            t1.normalize();
            t2 = cross(normal, t1);
            t2.normalize();
            
            //-------------------------------------------------------
            //Calculate bounce light contribution
            lastEvent = material.calculateRayCollision(factor,
                                                      indirectLight,
                                                      dir,
                                                      newDirection,
                                                      intersection,
                                                      normal,
                                                      t1,
                                                      t2,
                                                      initialized);

            //-------------------------------------------------------
            //Set the new values for the rays
            setDirection(newDirection);
            setOrigin(intersection);

            //-------------------------------------------------------
            //Next Event Estimation
            if(lastEvent == DIFFUSION){
                shadowEvent = castShadowRays(scene, directBounce, intersection, lastShape);
                if(shadowEvent != NO_EVENT){
                    if(initialized){
                        directLight = directLight + preFactor * directBounce;
                    }
                    else{
                        directLight = directLight + directBounce;
                    }
                }
            }
        }
    }while(closestShape != nullptr && lastEvent != ABSORPTION && lastEvent != LIGHTFOUND);
    //bounce when we have found something to bounce on, and neither a light source was found nor the ray was absorbed

    //---------------------------------------------------------------------------------------------------
    //Calculate final RGB tuple
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