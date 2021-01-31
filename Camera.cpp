#include "Camera.hpp"

//***********************************************************************
// Class default constructor
// Sets u = (0,1,0)
//      l = (1,0,0)
//      f = (0,0,1)
//      o = (0,0,0)
//***********************************************************************
Camera::Camera(){
    Direction u(0,1,0), l(1,0,0), f(0,0,1);
    Point o(0,0,0);
    this->up=u;
    this->left=l;
    this->front=f;
    this->origin=o;
}

//***********************************************************************
// Class constructor
// @param width Image width
// @param height Image height
// @param target Center point of the projection plane
// @param FOV Field of View to modify the front plane
// @param hfov if true, FOV represents horizontal field of view. Else, FOV represents vertical field of view.
//***********************************************************************
Camera::Camera(int width, int height, Point target, float FOV, bool hfov){
    Direction u(0,1,0), l(1,0,0), f(0,0,1);
    up = u;
    left = l;
    front = f;
    setAspectRatio(width, height);
    if(hfov) setHFOV(FOV);
    else     setVFOV(FOV);
    origin = target - front;

}

//***********************************************************************
// Class constructor
// @param u Camera's up vector
// @param l Camera's left vector
// @param f Camera's front vector
// @param o Camera's position
//***********************************************************************
Camera::Camera(Direction u, Direction l, Direction f, Point o){
    up = u;
    left = l;
    front = f;
    origin = o;
}

//***********************************************************************
// Sets origin
// @param p New camera's position
//***********************************************************************
void Camera::setOrigin(Point p){
    origin = p;
}

//***********************************************************************
// Sets the camera's system vectors
// @param u New camera's up
// @param l New camera's left
// @param f New camera's front
//***********************************************************************
void Camera::setCoord(Direction u, Direction l, Direction f){
    up = u;
    left = l;
    front = f;
}

//***********************************************************************
// Sets the camera's aspect ratio (modifies up vector, setting the relationship
// between up and left to the aspect ratio, given by the parameters)
// @param width Width of the new aspect ratio
// @param height Height of the new aspect ratio
//***********************************************************************
void Camera::setAspectRatio(int width, int height){
    up = up * (left.modulus() * height / width);
}

//***********************************************************************
// Sets the camera's horizontal field of view (modifies front vector)
// @param angle New horizontal FOV
//***********************************************************************
void Camera::setHFOV(float angle){
    float r = left.modulus() / (tan(angle/2));
    front = front * r;
}

//***********************************************************************
// Sets the camera's vertical field of view (modifies front vector)
// @param angle New vertical FOV
//***********************************************************************
void Camera::setVFOV(float angle){
    float r = up.modulus() / (tan(angle/2));
    front = front * r;
}

//***********************************************************************
// @returns Up vector
//***********************************************************************
Direction Camera::getUp(){
    return up;
}

//***********************************************************************
// @returns Left vector
//***********************************************************************
Direction Camera::getLeft(){
    return left;
}

//***********************************************************************
// @returns Front vector
//***********************************************************************
Direction Camera::getFront(){
    return front;
}

//***********************************************************************
// @returns Camera's origin
//***********************************************************************
Point Camera::getOrigin(){
    return origin;
}

//***********************************************************************
// @param p Point to be changed
// @returns A point resulting of changing the base using the camera base
//***********************************************************************
Point Camera::changeToGlobalCoordinates(Point p) const{
    return baseChange(p, left, up, front, origin);
}


//***********************************************************************
// @param d Direction to be changed
// @returns A direction resulting of changing the base using the camera base
//***********************************************************************
Direction Camera::changeToGlobalCoordinates(Direction d) const{
    return baseChange(d, left, up, front, origin);
}