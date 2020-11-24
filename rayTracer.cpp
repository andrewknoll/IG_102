#include "rayTracer.hpp"

using namespace std;

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

    unsigned currentIteration = 0;

    Ray ray;
    ray.setOrigin(o);

    if(nShapes > 0){
        cout << fixed << setprecision(2);
        cout << "Applying Ray Tracing Algorithm to the scene..." << endl;
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                for(int r = 0; r < rpp; r++){
                    /*Set d to a point of the projection plane (between -1 and 1)
                     *  The projection plane maps the image:
                     *      image[i, j] = projectionPlane[x, y] 
                     *
                     *      Relationship?
                     *          (x, y) = (i*a + b, j*c + d)   => We must find a, b, c, d
                     * 
                     *      nx and ny are the dimensions of the image
                     *      image[0, 0] = projectionPlane[-1, -1]
                     *          => (-1, -1) = (b, d)
                     *          => b = -1; d = -1
                     * 
                     * 
                     *      image[nx, ny] = projectionPlane[1, 1]  => (1, 1) = (nx*a -1, ny*c -1)
                     * 
                     *      Therefore:
                     *          nx*a -1 = 1
                     *              => nx*a = 2
                     *              => a = 2/nx
                     * 
                     *          ny*c -1 = 1
                     *              => ny*c = 2
                     *              => c = 2/ny
                     * 
                     *          x = i*(2/nx) -1
                     *          y = j*(2/ny) -1
                     * 
                     *          image[i, j] = projectionPlane[2i/nx -1, 2j/ny -1]
                     *          we will also add a random factor for antialias            
                     */
                    d.setAll((2*i + rng.getNumber(0,1))/width - 1 , (2*j + rng.getNumber(0,1))/height -1 , 1);
                    d.normalize();
                    d = camera.changeToGlobalCoordinates(d);

                    ray.setDirection(d);    //Set the new direction as the ray's direction
                    rayResult[r] = ray.getRayResult(scene); //Take RGB component found by the ray

                    currentIteration += nShapes;
                }
                cout << "\r " << currentIteration * 100.0 / iterationsNecessary  << "%";
                img.setTuple(calculateRGBMean(rayResult), i, j);
            }
        }
    }
    cout << "\r 100.00%" << endl;
}