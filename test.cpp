#include "Scene.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Plane.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Point.hpp"
#include "color/RGB.hpp"
#include <math.h>
#include "pathTracer.hpp"
#include "LightPoint.hpp"
#include "color/ImageWriter.hpp"
#include <memory>


Scene scene1(){
    Direction a(0, 0, 25);
    Point p(0, 0, 70);
    Point point2(17, 0, 60);
    Point point3(-30, 0, 90);
    RGB rgb(0.9, 0 , 0);
    Point o(0, 0, -6);
    RGB black(0, 0, 0);

    Direction n(0, 0, -1);
    RGB rgb4(0, 0.5, 0.5);

    RGB rgb2(0.25, 0.75, 0.16);
    RGB rgb5(0.25, 1, 0.16);
    RGB rgb3(1, 1, 1);

    Material diffuse;
    diffuse.setAsDiffuse(rgb);

    Material dielectric;
    dielectric.setAsDielectric(rgb3, rgb3);

    Material plastic;
    plastic.setAsPlastic(rgb2, rgb5);

    Material emission;
    emission.setAsLightSource(rgb3);

    Scene scene(400, 400);
    scene.buildCameraFromVFOV(M_PI/2, o);

    shared_ptr<Sphere> p1 = make_shared<Sphere>(a, p);
    shared_ptr<Plane> p2 = make_shared<Plane>(n, 20);
    shared_ptr<Sphere> p3 = make_shared<Sphere>(a, point2);
    shared_ptr<Sphere> p4 = make_shared<Sphere>(a, point3);

    p1->setMaterial(emission);
    p2->setMaterial(plastic);
    p3->setMaterial(diffuse);
    p4->setMaterial(diffuse);

    scene.addShape(p1);
    scene.addShape(p2);
    scene.addShape(p3);
    scene.addShape(p4);

    return scene;
}

Scene scene2(){
    Scene scene(400, 400);
    scene.buildCameraFromHFOV(M_PI/2, Point(0,0,0));
    shared_ptr<Plane> floor = make_shared<Plane>(Direction(0, 1, 0), 10);
    AreaLight light(Point(0, 9, 10), Direction(1,0,0), Direction(0,0,1), 3, 2, RGB(50,50,50));
    shared_ptr<Plane> ceiling = make_shared<Plane>(Direction(0, 1, 0), -10);
    shared_ptr<Plane> redWall = make_shared<Plane>(Direction(1, 0, 0), 10);
    shared_ptr<Plane> greenWall = make_shared<Plane>(Direction(1, 0, 0), -10);
    shared_ptr<Plane> whiteWall = make_shared<Plane>(Direction(0, 0, -1), 14);
    shared_ptr<Sphere> plasticSphere = make_shared<Sphere>(Direction(0, 0, 4), Point(3.5, -8, 10));
    shared_ptr<Sphere> specularSphere = make_shared<Sphere>(Direction(0, 0, 4), Point(-4, -8, 13));

    Material diffuseWhite;
    diffuseWhite.setAsDiffuse(RGB(1, 1, 1));

    Material diffuseRed;
    diffuseRed.setAsDiffuse(RGB(1, 0, 0));

    Material diffuseGreen;
    diffuseGreen.setAsDiffuse(RGB(0, 1, 0));

    Material plastic;
    plastic.setAsPlastic(RGB(1, 0, 0), RGB(1, 0, 0));

    Material dielectric;
    dielectric.setAsDielectric(RGB(1,1,1), RGB(1,1,1));
    
    floor->setMaterial(diffuseWhite);
    plasticSphere->setMaterial(plastic);
    specularSphere->setMaterial(dielectric);
    whiteWall->setMaterial(diffuseWhite);
    redWall->setMaterial(diffuseRed);
    greenWall->setMaterial(diffuseGreen);
    ceiling->setMaterial(diffuseWhite);

    scene.addShape(floor);
    scene.addShape(plasticSphere);
    scene.addShape(specularSphere);
    scene.addShape(whiteWall);
    scene.addShape(redWall);
    scene.addShape(greenWall);
    scene.addShape(ceiling);
    scene.addLight(light);
    
    return scene;
}

Scene scene3(){
    Scene scene(400, 400);
    scene.buildCameraFromHFOV(2*M_PI/5, Point(0,0,-3));

    shared_ptr<Plane> floor = make_shared<Plane>(Direction(2, 2, -5), 27);
    shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Direction(2, 0, 0), Point(-2, -1, 2));
    shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Direction(0, 3, 0), Point(0, -0.5, 4));
    shared_ptr<Sphere> sphere3 = make_shared<Sphere>(Direction(0, 0, 1), Point(3, -0.75, 5));
    LightPoint lp(Point(-2, 2, -7), RGB(100, 100, 100));

    Material plastic1;
    plastic1.setAsPlastic(RGB(0.1,0.5,0.5), RGB(0.2,0.2,1));
    Material plastic2;
    plastic2.setAsPlastic(RGB(0.5,0.05,0.05), RGB(0.5,0.05,0.05));
    Material plastic3;
    plastic3.setAsPlastic(RGB(0.5,0.5,0.05), RGB(0.5,0.5,0.05));
    Material plastic4;
    plastic4.setAsPlastic(RGB(0.2,0.2,0.5), RGB(0.2,0.2,0.5));

    floor->setMaterial(plastic1);
    sphere1->setMaterial(plastic2);
    sphere2->setMaterial(plastic3);
    sphere3->setMaterial(plastic4);

    scene.addShape(floor);
    scene.addShape(sphere1);
    scene.addShape(sphere2);
    scene.addShape(sphere3);
    scene.addLight(lp);

    return scene;

}

int main(){

    Image img;
    img.setWidthHeight(400, 400);

    pathTrace(img, scene2(), 64);

    float args[2] = {img.getMax(), 0.95};
    writeImage(img, "output.bmp", GAMMA, 255, true, args);

    
}