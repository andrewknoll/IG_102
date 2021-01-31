#include "Scene.hpp"

//***********************************************************************
// Class constructor
// @param width Width of the scene
// @param height Height of the scene
//***********************************************************************
Scene::Scene(int width, int height){
    this->width = width;
    this->height = height;
}

//***********************************************************************
// Class constructor
// @param width Width of the scene
// @param height Height of the scene
// @param c Camera used for rendering
//***********************************************************************
Scene::Scene(int width, int height, Camera c){
    this->width = width;
    this->height = height;
    this->camera = c;
}

//***********************************************************************
// @returns Scene's width
//***********************************************************************
int Scene::getWidth(){
    return this->width;
}

//***********************************************************************
// @returns Scene's height
//***********************************************************************
int Scene::getHeight(){
    return this->height;
}

//***********************************************************************
// Sets a new value for the width
// @param width New width
//***********************************************************************
void Scene::setWidth(int width){
    this->width = width;
}

//***********************************************************************
// Sets a new value for the height
// @param height New height
//***********************************************************************
void Scene::setHeight(int height){
    this->height = height;
}

//***********************************************************************
// Sets a new camera
// @param camera New camera
//***********************************************************************
void Scene::setCamera(Camera camera){
    this->camera = camera;
}

//***********************************************************************
// Adds a shape to the scene
// @param s Smart pointer to the shape to be added
//***********************************************************************
void Scene::addShape(shared_ptr<Shape> s){
    objects.emplace_back(s);
}

//***********************************************************************
// Adds a light to the scene
// @param lp Light point to be added
//***********************************************************************
void Scene::addLight(LightPoint lp){
    lightPoints.emplace_back(lp);
}

//***********************************************************************
// Adds a light to the scene
// @param al Area light to be added
//***********************************************************************
void Scene::addLight(AreaLight al){
    areaLights.emplace_back(al);
}

//***********************************************************************
// Creates a camera from the Horizontal field of view and the target point of the projection plane
// @param HFOV Horizontal field of view
// @param target Target point of camera's projection plane
//***********************************************************************
void Scene::buildCameraFromHFOV(float HFOV, Point target){
    Camera c(width, height, target, HFOV, true);
    camera = c;
}

//***********************************************************************
// Creates a camera from the Vertical field of view and the target point of the projection plane
// @param VFOV Horizontal field of view
// @param target Target point of camera's projection plane
//***********************************************************************
void Scene::buildCameraFromVFOV(float VFOV, Point target){
    Camera c(width, height, target, VFOV, false);
    camera = c;
}

//***********************************************************************
// @returns Scene's camera's target
//***********************************************************************
Point Scene::getTarget(){
    return camera.getFront() + camera.getOrigin();
}

//***********************************************************************
// @returns Scene's camera
//***********************************************************************
Camera Scene::getCamera(){
    return camera;
}

//***********************************************************************
// Gets a shape by an index
// @param i Index of shape
// @returns Shape indexed by i
//***********************************************************************
shared_ptr<Shape> Scene::getShape(int i){
    return static_pointer_cast<Shape>(objects[i]);
}

//***********************************************************************
// Gets a Light point by an index
// @param i Index of light
// @returns Light point indexed by i
//***********************************************************************
LightPoint Scene::getLightPoint(int i){
    return lightPoints[i];
}

//***********************************************************************
// Gets an Area Light by an index
// @param i Index of light
// @returns Light indexed by i
//***********************************************************************
AreaLight Scene::getAreaLight(int i){
    return areaLights[i];
}

//***********************************************************************
// @returns Number of shapes in the scene
//***********************************************************************
int Scene::shapeN(){
    return objects.size();
}

//***********************************************************************
// @returns Number of light points in the scene
//***********************************************************************
int Scene::lightPN(){
    return lightPoints.size();
}

//***********************************************************************
// @returns Number of area lights in the scene
//***********************************************************************
int Scene::areaLightN(){
    return areaLights.size();
}

////////////////////////////////////////////////

