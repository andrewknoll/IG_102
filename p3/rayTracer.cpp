#include "rayTracer.hpp"
#include <random>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>

using namespace std;

const double MAX_DOUBLE = numeric_limits<double>::max();

void rayTrace(Image& img, Scene scene, int rpp){
    Direction d;
    Point rayObjective;
    randomNumberGen rng;
    vector<RGB> rayResult(rpp);
    
    int width = scene.getWidth();
    int height = scene.getHeight();
    Camera camera = scene.getCamera();
    Point o = camera.getOrigin();
    //Point target = scene.getTarget();
    int nShapes = scene.length();
    unsigned int iterationsNecessary = width * height * rpp * nShapes;


    float offset[2] = {height/2.0f, width/2.0f};

    struct PossibleSolution<Point> ps[2];

    double minDistance = MAX_DOUBLE;
    shared_ptr<Shape> closestShape = nullptr;

    float currentDistance;
    Point currentPoint;
    unsigned currentIteration = 0;

    

    if(nShapes > 0){
        cout << fixed << setprecision(2);
        cout << "Applying Ray Tracing Algorithm to the scene..." << endl;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                for(int r = 0; r < rpp; r++){
                    d.setAll((j + rng.getNumber() - offset[0])/height , (i + rng.getNumber() - offset[1])/width , 1);
                    d.normalize();
                    d = camera.changeToGlobalCoordinates(d);

                    closestShape = nullptr; //Reset closest shape
                    minDistance = MAX_DOUBLE;   //Reset minimum distance

                    for(int s = 0; s < nShapes; s++){ //Iterate through all shapes in scene
                        scene.getShape(s)->findFirstIntersectionWithLine(d, o, ps); //Find intersection with ray
                    
                        if(ps[0].doesExist()){ //If solution is real

                            //Do one iteration if only one solution has been found
                            //Do two if two solutions have been found
                            for(int sol = 0; sol < (ps[1].doesExist() ? 2: 1); sol++){
                                currentPoint = ps[sol].getSolution();    //Get solution
                                currentDistance = distance(currentPoint, o);    //Get distance to origin

                                if(minDistance > currentDistance && currentPoint.getZ() >= o.getZ()){    //If distance is the smallest found AND object is not behind the camera
                                    minDistance = currentDistance;  //Update minimum distance
                                    closestShape = scene.getShape(s);    //Update closest shape to camera
                                }
                            }
                            
                        }
                        currentIteration++;
                    }

                    if(closestShape != nullptr){    //If any intersection was found with ray
                        rayResult[r] = closestShape->getEmission(); //Set ray's color to the closest object's emission
                    }
                    
                    else{
                        rayResult[r] = RGB(0, 0, 0);    //otherwise set it to black
                    }
                }
                cout << "\r " << currentIteration * 100.0 / iterationsNecessary  << "%";
                img.setTuple(calculateRGBMean(rayResult), i, j);
            }
        }
    }
    cout << "\r 100.00%" << endl;
}