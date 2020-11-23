#include "Camera.hpp"

Camera::Camera(){
    Direction u(0,1,0), l(1,0,0), f(0,0,1);
    Point o(0,0,0);
    this->up=u;
    this->left=l;
    this->front=f;
    this->origin=o;
}

Camera::Camera(int width, int height, Point target, double FOV, bool hfov){
    Direction u(0,1,0), l(1,0,0), f(0,0,1);
    up = u;
    left = l;
    front = f;
    setAspectRatio(width, height);
    if(hfov) setHFOV(FOV);
    else     setVFOV(FOV);
    origin = target - front;

}

Camera::Camera(Direction u, Direction l, Direction f, Point o){
    up = u;
    left = l;
    front = f;
    origin = o;
}

void Camera::setOrigin(Point p){
    origin = p;
}

void Camera::setCoord(Direction u, Direction l, Direction f){
    up = u;
    left = l;
    front = f;
}

void Camera::setAspectRatio(int width, int height){
    up = up * (left.modulus() * height / width);
}

void Camera::setHFOV(double angle){
    double r = left.modulus() / (tan(angle/2));
    front = front * r;
}

void Camera::setVFOV(double angle){
    double r = up.modulus() / (tan(angle/2));
    front = front * r;
}

Direction Camera::getUp(){
    return up;
}

Direction Camera::getLeft(){
    return left;
}

Direction Camera::getFront(){
    return front;
}

Point Camera::getOrigin(){
    return origin;
}

Point Camera::changeToGlobalCoordinates(Point p){
    return baseChange(p, origin, left, up, front);
}

Direction Camera::changeToGlobalCoordinates(Direction d){
    return baseChange(d, origin, left, up, front);
}