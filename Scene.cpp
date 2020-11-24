#include "Scene.hpp"

Scene::Scene(int width, int height){
    this->width = width;
    this->height = height;
}

Scene::Scene(int width, int height, Camera c){
    this->width = width;
    this->height = height;
    this->camera = c;
}

int Scene::getWidth(){
    return this->width;
}

int Scene::getHeight(){
    return this->height;
}

void Scene::setWidth(int width){
    this->width = width;
}

void Scene::setHeight(int height){
    this->height = height;
}

void Scene::setCamera(Camera camera){
    this->camera = camera;
}

void Scene::addShape(shared_ptr<Shape> s){
    objects.emplace_back(s);
}


void Scene::buildCameraFromHFOV(float HFOV, Point target){
    Camera c(width, height, target, HFOV, true);
    camera = c;
}

void Scene::buildCameraFromVFOV(float VFOV, Point target){
    Camera c(width, height, target, VFOV, false);
    camera = c;
}

Point Scene::getTarget(){
    return camera.getFront() + camera.getOrigin();
}

Camera Scene::getCamera(){
    return camera;
}

shared_ptr<Shape> Scene::getShape(int i){
    return static_pointer_cast<Shape>(objects[i]);
}

int Scene::length(){
    return objects.size();
}

////////////////////////////////////////////////
