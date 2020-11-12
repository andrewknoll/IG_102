#include "Camera.hpp"

    Camera(){
        up = new Direction();
        left =  new Direction();
        front = new Direction();
        origin = new Point();
        width = 0;
        heigth = 0;
        raysPP = 0;
    } 
    
    Camera(Scene scene){
        // Ni putis idea
    }

    void setOrigin(Point p){
        origin = p;
    }
    void setResolution(int h, int w){
        heigth = h;
        width = w;
    }
    void setCoord(Direction u, Direction l, Direction f){
        up = u;
        left = l;
        front = f;
    }
    void setRaysPP(int r){
        raysPP = r;
    }
    Imagen rayTracing(){
        // JA JA JA
    }