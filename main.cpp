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
#include "globals.hpp"
#include <chrono>
#define N_SCENES 11

using ScenePtr = shared_ptr<Scene>;

//***********************************************************************
// Creates a scene with a lambertian sphere, a dielectric sphere, a plastic sphere and a light
// @param lp If set to true: a light point will be used. If set to false: an emitting sphere will be used.
// @returns a pointer to the described scene
//***********************************************************************
ScenePtr scene1(bool lp){
    RGB rgb1(0,0,0.9);
    RGB refrac(1.5,1.5,1.5);
    RGB rgb2(0.3,0.3,0);
    RGB e(20,20,20);
    RGB rgb5(0.6,0.6,0.6);

    Point o(0,0,0);
    Direction a(0,1.5,0);

    Point p1(0,2,2);
    Point p2(1,-0.5,1.5);
    Point p3(-1,-0.5,1.5);

    Direction n(0,0.1,1);


    Material diffuse;
    diffuse.setAsLambertian(rgb1);

    Material diffuse2;
    diffuse2.setAsLambertian(RGB(0.9,0.6,0.9));

    Material diffuse3;
    diffuse3.setAsLambertian(RGB(0.2,0.2,0.2));

    Material mirror;
    mirror.setAsPlastic(RGB(0.05,0.06,0.05), RGB(0.8, 0.8, 0.8));

    Material plastic;
    plastic.setAsPlastic(rgb2, rgb5);

    Material emission;
    emission.setAsLightSource(e);

    LightPoint lightpoint(Point(0, 2, 2), RGB(30, 30, 30));

    ScenePtr scene = make_shared<Scene>(400, 400);
    scene->buildCameraFromVFOV(M_PI/2, o);

    shared_ptr<Sphere> s1 = make_shared<Sphere>(a, p1);
    shared_ptr<Plane> s2 = make_shared<Plane>(n, 10);
    shared_ptr<Sphere> s3 = make_shared<Sphere>(a, p2);
    shared_ptr<Sphere> s4 = make_shared<Sphere>(a, p3);
    shared_ptr<Plane> s5 = make_shared<Plane>(Direction(0,1,0.01), 1);
    shared_ptr<Plane> s6 = make_shared<Plane>(Direction(-0.5,0,-1), -3);
    shared_ptr<Sphere> s7 = make_shared<Sphere>(Direction(25, 25, 25), scene->getCamera().getOrigin());

    s1->setMaterial(emission);
    s2->setMaterial(plastic);
    s3->setMaterial(diffuse);
    s4->setMaterial(mirror);
    s5->setMaterial(diffuse2);
    s6->setMaterial(diffuse2);
    s7->setMaterial(diffuse3);

    if(lp) scene->addLight(lightpoint);
    else scene->addShape(s1);
    scene->addShape(s2);
    scene->addShape(s3);
    scene->addShape(s4);
    scene->addShape(s5);
    scene->addShape(s6);
    scene->addShape(s7);

    return scene;
}

//***********************************************************************
// Creates a Cornell box scene with a light
// @param lp If set to true: a light point will be used. If set to false: an area light will be used.
// @returns a pointer to the described scene
//***********************************************************************
ScenePtr scene2(bool lp){
    ScenePtr scene = make_shared<Scene>(400, 400);
    scene->buildCameraFromHFOV(3*M_PI/5, Point(0,0,0));

    shared_ptr<Plane> ceiling = make_shared<Plane>(Direction(0, 1, 0), -5);
    AreaLight arealight(Point(0, 4.99, 2.5), Direction(1,0,0), Direction(0,0,1), 3, 2, RGB(5,5,5));
    LightPoint lightpoint(Point(0, 4.99, 2), RGB(30,30,30));
    shared_ptr<Plane> floor = make_shared<Plane>(Direction(0, 1, 0), 5);
    shared_ptr<Plane> redWall = make_shared<Plane>(Direction(1, 0, 0), -5);
    shared_ptr<Plane> greenWall = make_shared<Plane>(Direction(1, 0, 0), 5);
    shared_ptr<Plane> whiteWall = make_shared<Plane>(Direction(0, 0, -1), 5);
    shared_ptr<Sphere> plasticSphere = make_shared<Sphere>(Direction(0, 0, 3), Point(2.5, -4, 5));
    shared_ptr<Sphere> specularSphere = make_shared<Sphere>(Direction(0, 0, 2), Point(-3, -2, 4));

    Material diffuseWhite;
    diffuseWhite.setAsLambertian(RGB(0.9, 0.9, 0.9));

    Material diffuseRed;
    diffuseRed.setAsLambertian(RGB(0.9, 0, 0));

    Material diffuseGreen;
    diffuseGreen.setAsLambertian(RGB(0, 0.9, 0));

    Material plastic;
    plastic.setAsPlastic(RGB(0.6, 0, 0), RGB(0.3, 0.3, 0.3));

    Material specular;
    specular.setAsPlastic(RGB(0,0,0), RGB(0.9, 0.9, 0.9));
    
    floor->setMaterial(diffuseWhite);
    plasticSphere->setMaterial(plastic);
    specularSphere->setMaterial(specular);
    whiteWall->setMaterial(diffuseWhite);
    redWall->setMaterial(diffuseRed);
    greenWall->setMaterial(diffuseGreen);
    ceiling->setMaterial(diffuseWhite);

    scene->addShape(floor);
    scene->addShape(plasticSphere);
    scene->addShape(specularSphere);
    scene->addShape(whiteWall);
    scene->addShape(redWall);
    scene->addShape(greenWall);
    scene->addShape(ceiling);
    if(lp) scene->addLight(lightpoint);
    else scene->addLight(arealight);
    
    return scene;
}

//***********************************************************************
// Creates a scene with a plastic plane and 3 plastic spheres.
// Has a light point
// @returns a pointer to the described scene
//***********************************************************************
ScenePtr scene3(){
    ScenePtr scene = make_shared<Scene>(400, 400);
    scene->buildCameraFromHFOV(2*M_PI/5, Point(0,0,-3));

    shared_ptr<Plane> floor = make_shared<Plane>(Direction(0.1, 5, 0.1), 2);
    shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Direction(2, 0, 0), Point(-2, -1, 2));
    shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Direction(0, 3, 0), Point(0, -0.5, 4));
    shared_ptr<Sphere> sphere3 = make_shared<Sphere>(Direction(0, 0, 1), Point(3, -0.75, 5));
    LightPoint lp(Point(-2, 2, -7), RGB(100, 100, 100));

    Material plastic;
    plastic.setAsPlastic(RGB(0.1,0.49,0.49), RGB(0.5,0.5,0.5));
    Material lambertian1;
    lambertian1.setAsLambertian(RGB(0.4,0.05,0.05));
    Material lambertian2;
    lambertian2.setAsLambertian(RGB(0.4,0.4,0.05));
    Material lambertian3;
    lambertian3.setAsLambertian(RGB(0.15,0.15,0.4));

    floor->setMaterial(plastic);
    sphere1->setMaterial(lambertian1);
    sphere2->setMaterial(lambertian2);
    sphere3->setMaterial(lambertian3);

    scene->addShape(floor);
    scene->addShape(sphere1);
    scene->addShape(sphere2);
    scene->addShape(sphere3);
    scene->addLight(lp);

    return scene;

}

//***********************************************************************
// Creates a scene with a plane, a light point and an area light
// Has a light point
// @returns a pointer to the described scene
//***********************************************************************
ScenePtr scene4(MaterialType mt){
    ScenePtr scene = make_shared<Scene>(400, 400);
    scene->buildCameraFromHFOV(3*M_PI/5, Point(0,0,0));

    Direction base(1,0,0);
    base.normalize();
    Direction height(0,0.1,1);
    height.normalize();

    AreaLight arealight(Point(-2, 0, 2.5), base, height, 1, 1, RGB(0,0,5));
    LightPoint lightpoint(Point(3, 0, 2), RGB(30,30,30));

    Material diffusePink;
    diffusePink.setAsLambertian(RGB(0.9, 0.68, 0.72));

    Material mirror;
    mirror.setAsPlastic(RGB(0, 0, 0), RGB(0.9,0.9,0.9));

    Material glass;
    glass.setAsDielectric(0.1, RGB(1.655, 1.655, 1.655));

    Material plasticYellow;
    plasticYellow.setAsPlastic(RGB(0.6, 0.6, 0), RGB(0.3, 0.3, 0.3));

    Material emissionCyan;
    emissionCyan.setAsLightSource(RGB(0, 3, 3));


    shared_ptr<Plane> floor = make_shared<Plane>(Direction(0, 1, 0), 5);
    shared_ptr<Plane> wall = make_shared<Plane>(Direction(-0.6, 0, -0.8), 6);
    shared_ptr<Plane> wall2 = make_shared<Plane>(Direction(-0.6, 0, -0.8), 6.02);
    shared_ptr<Sphere> plasticSphere = make_shared<Sphere>(Direction(0, 0, 3), Point(0, -4, 5));
    shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Direction(0, 0, 5), Point(5, 1, 16));

    plasticSphere->setMaterial(plasticYellow);
    sphere2->setMaterial(emissionCyan);
    floor->setMaterial(diffusePink);
    wall2->setMaterial(glass);

    switch(mt){
        case LAMBERTIAN:
            wall->setMaterial(diffusePink);
            break;
        case PLASTIC:
            wall->setMaterial(mirror);
            break;
        case DIELECTRIC:
            wall->setMaterial(glass);
            break;
        default:
            break;
    }

    scene->addShape(floor);
    scene->addShape(wall);
    scene->addLight(arealight);
    scene->addLight(lightpoint);
    scene->addShape(plasticSphere);
    scene->addShape(sphere2);
    scene->addShape(wall2);

    return scene;
}

ScenePtr scene5(){
    ScenePtr scene = make_shared<Scene>(400, 400);
    scene->buildCameraFromHFOV(3*M_PI/5, Point(0,0,0));

    shared_ptr<Plane> ceiling = make_shared<Plane>(Direction(0, 1, 0), -5);
    AreaLight arealight(Point(0, 4.99, 2.5), Direction(1,0,0), Direction(0,0,1), 3, 2, RGB(5,5,5));
    LightPoint lightpoint(Point(0, 4.99, 2), RGB(30,30,30));
    shared_ptr<Plane> floor = make_shared<Plane>(Direction(0, 1, 0), 5);
    shared_ptr<Plane> redWall = make_shared<Plane>(Direction(1, 0, 0), -5);
    shared_ptr<Plane> greenWall = make_shared<Plane>(Direction(1, 0, 0), 5);
    shared_ptr<Plane> whiteWall = make_shared<Plane>(Direction(0, 0, -1), 5);
    shared_ptr<Sphere> dielectricSphere = make_shared<Sphere>(Direction(0, 0, 5), Point(0, 0, 3));

    Material diffuseWhite;
    diffuseWhite.setAsLambertian(RGB(0.9, 0.9, 0.9));

    Material diffuseRed;
    diffuseRed.setAsLambertian(RGB(0.9, 0, 0));

    Material diffuseGreen;
    diffuseGreen.setAsLambertian(RGB(0, 0.9, 0));

    Material dielectric;
    dielectric.setAsDielectric(0.05, RGB(1.2, 1.2, 1.2));
    
    floor->setMaterial(diffuseWhite);
    dielectricSphere->setMaterial(dielectric);
    whiteWall->setMaterial(diffuseWhite);
    redWall->setMaterial(diffuseRed);
    greenWall->setMaterial(diffuseGreen);
    ceiling->setMaterial(diffuseWhite);

    scene->addShape(floor);
    scene->addShape(dielectricSphere);
    scene->addShape(whiteWall);
    scene->addShape(redWall);
    scene->addShape(greenWall);
    scene->addShape(ceiling);
    scene->addLight(lightpoint);
    scene->addLight(arealight);
    
    return scene;

}


ScenePtr scene6(bool useFresnel){
    ScenePtr scene = make_shared<Scene>(400, 400);
    scene->buildCameraFromHFOV(3*M_PI/5, Point(0,0,0));

    vector<shared_ptr<Sphere>> dielectricSpheres(5);

    shared_ptr<Plane> ceiling = make_shared<Plane>(Direction(0, 1, 0), -5);
    AreaLight arealight(Point(0, 4.99, 2.5), Direction(1,0,0), Direction(0,0,1), 3, 2, RGB(5,5,5));
    LightPoint lightpoint(Point(-4.99, 4, 2), RGB(30,30,30));
    LightPoint lightpoint2(Point(4.99, 4, 2), RGB(30,30,30));
    shared_ptr<Plane> floor = make_shared<Plane>(Direction(0, 1, 0), 5);
    shared_ptr<Plane> redWall = make_shared<Plane>(Direction(1, 0, 0), -5);
    shared_ptr<Plane> blueWall = make_shared<Plane>(Direction(1, 0, 0), 5);
    shared_ptr<Plane> whiteWall = make_shared<Plane>(Direction(0, 0, -1), 5);

    Material diffuseWhite;
    diffuseWhite.setAsLambertian(RGB(0.9, 0.9, 0.9));

    Material diffuseRed;
    diffuseRed.setAsLambertian(RGB(0.9, 0, 0));

    Material diffuseBlue;
    diffuseBlue.setAsLambertian(RGB(0, 0, 0.9));

    Material dielectric;
    dielectric.setAsDielectric(0.05, RGB(1.2, 1.2, 1.2));

    Material custom;
    if(!useFresnel){
        custom.setAsCustomMaterial(RGB(0,0,0), RGB(0.1,0.1,0.1), RGB(1.2,1.2,1.2));
    }

    for(int i = 0; i < 5; i++){
        dielectricSpheres[i] = make_shared<Sphere>(Direction(0, 0, 1), Point(i*1.2-3, -4, 4));
        if(useFresnel) dielectricSpheres[i]->setMaterial(dielectric);
        else dielectricSpheres[i]->setMaterial(custom);
        scene->addShape(dielectricSpheres[i]);
    }
    
    floor->setMaterial(diffuseWhite);
    whiteWall->setMaterial(diffuseWhite);
    redWall->setMaterial(diffuseRed);
    blueWall->setMaterial(diffuseBlue);
    ceiling->setMaterial(diffuseWhite);

    scene->addShape(floor);
    scene->addShape(whiteWall);
    scene->addShape(redWall);
    scene->addShape(blueWall);
    scene->addShape(ceiling);
    scene->addLight(lightpoint);
    scene->addLight(lightpoint2);
    scene->addLight(arealight);
    
    return scene;

}

//***********************************************************************
// Executes the path tracer with the arguments provided
//***********************************************************************
int main(int argc, char* argv[]){

    //Default Values
    string filename = "output.ppm";
    bool customFilename = false;
    int width = 400, height = 400;
    int scene = 2;
    int rpp = 10;
    int colorRes = 255;
    int toneMapper = 5; //Reinhard 2002
    bool bmp = false;
    float tma1, tma2;

    ScenePtr s;

    PathTracer pt;

    //-----------------------------------------------------------------------------
    //Parse terminal arguments
    for(int i = 1; i < argc; i++){
        if(strcmp("-output-name", argv[i])==0){
            filename = argv[++i];
            customFilename = true;
            if(filename == ""){
                cerr << "Invalid filename parameter" << endl;
                exit(1);
            }
        }
        else if(strcmp("-width", argv[i])==0){
            width = atoi(argv[++i]);
            if(width < 0){
                cerr << "Invalid width parameter" << endl;
                exit(1);
            }
        }
        else if(strcmp("-height", argv[i])==0){
            height = atoi(argv[++i]);
            if(height < 0){
                cerr << "Invalid height parameter" << endl;
                exit(1);
            }
        }
        else if(strcmp("-scene", argv[i])==0){
            scene = atoi(argv[++i]);
            if(scene < 1 || scene > N_SCENES){
                cerr << "Invalid scene parameter" << endl;
                exit(1);
            }
        }
        else if(strcmp("-rpp", argv[i])==0){
            rpp = atoi(argv[++i]);
            if(scene < 1 || scene > N_SCENES){
                cerr << "Invalid scene parameter" << endl;
                exit(1);
            }
        }
        else if(strcmp("-tone-mapping", argv[i])==0){
            toneMapper = atoi(argv[++i]);
            if(toneMapper < 0 || toneMapper >= N_TONEMAPPERS){
                cerr << "Invalid tone mapper parameter" << endl;
                exit(1);
            }
            else{
                if(toneMapper == EQUALIZECLAMP || toneMapper == CLAMPGAMMA){
                    tma1 = atof(argv[++i]);
                }
                if(toneMapper == GAMMA || toneMapper == CLAMPGAMMA){
                    tma2 = atof(argv[++i]);
                }
            }
        }
        else if(strcmp("-bmp", argv[i])==0){
            bmp = true;
        }
        else if(strcmp("-color-resolution", argv[i])==0){
            colorRes = atoi(argv[++i]);
        }
    }

    if(colorRes <= 8 && bmp){
        cerr << "BMP currently not supported with less than 8 colours" << endl;
        exit(1);
    }
    if(!customFilename && bmp){
        filename = "output.bmp";
    }

    switch(scene){
        case 1:
            s = scene1(false);
            break;
        case 2:
            s = scene1(true);
            break;
        case 3:
            s = scene2(false);
            break;
        case 4:
            s = scene2(true);
            break;
        case 5:
            s = scene3();
            break;
        case 6:
            s = scene4(LAMBERTIAN);
            break;
        case 7:
            s = scene4(PLASTIC);
            break;
        case 8:
            s = scene4(DIELECTRIC);
            break;
        case 9:
            s = scene5();
            break;
        case 10:
            s = scene6(true);
            break;
        case 11:
            s = scene6(false);
            break;
    }
    //-----------------------------------------------------------------------------
    //Path trace
    Image img;
    img.setWidthHeight(width, height);

    auto start = chrono::high_resolution_clock::now(); 
    pt.pathTrace(img, *s, rpp);
    auto stop = chrono::high_resolution_clock::now(); 

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    if(toneMapper == EQUALIZE || toneMapper == GAMMA || toneMapper == REINHARD2005){
        tma1 = img.getMaxFound();
    }
    //-----------------------------------------------------------------------------
    //Tone map and write to output
    float toneMapperArgs[2] = {tma1, tma2};
    writeImage(img, filename, (ToneMappingType)toneMapper, colorRes, bmp, toneMapperArgs);

    cout << duration.count() << " Milliseconds\a" << endl;

    
}