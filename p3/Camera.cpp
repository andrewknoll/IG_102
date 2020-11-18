#include "Camera.hpp"

Camera::Camera(int width, int height, Point target, double FOV, bool hfov){
    Direction up(1,0,0), left(0,1,0), front(0,0,1);
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
    up = left * height / width;
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