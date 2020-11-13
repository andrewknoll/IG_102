#include "Camera.hpp"

    Camera::Camera(){
        up = new Direction(1,0,0);
        left =  new Direction(0,1,0);
        front = new Direction(0,0,8);
        origin = new Point(0,0,0);
        width = 0;
        heigth = 0;
        raysPP = 0;
    } 
    
    Camera::Camera(Scene scene){
        // Ni putis idea
        int sceneHeigth = scene.getHeigth();
        Point sceneCenter = scene.getCenter();
        // Posicionar el centro de la camara a 4(alturas de escena) del centro de la escena
        //¿EN QUE ANGULO? XD
    }

    void Camera::setOrigin(Point* p){
        origin = p;
    }
    void Camera::setResolution(int h, int w){
        heigth = h;
        width = w;
    }
    void Camera::setCoord(Direction *u, Direction *l, Direction *f){
        up = u;
        left = l;
        front = f;
    }
    void Camera::setRaysPP(int r){
        raysPP = r;
    }
    void Camera::rayTracing(){
        // JA JA JA
    }