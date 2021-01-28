#include "pathTracer.hpp"

using namespace std;

void PathTracer::pathTrace(Image& img, Scene scene, int rpp){
    
    

    Point rayObjective;
    
    int width = scene.getWidth();
    int height = scene.getHeight();
    //Point target = scene.getTarget();
    int nShapes = scene.shapeN();

    unsigned int subdivisions;

    pixelsLeftToGenerate = width * height;

    if(nShapes > 0){
        int nThreads = thread::hardware_concurrency() - 1;

        subdivisions = numberOfSubdivisions(width, height);

        cout << fixed << setprecision(2);
        cout << "Applying Path Tracing Algorithm to the scene with " << nThreads + 1 << " processes..." << endl;

        if(nThreads > 1){
            vector<thread> pool(nThreads);

            for(int i = 0; i < nThreads; i++){
            pool[i] = thread(&PathTracer::threadWork, this, ref(img), scene, rpp, subdivisions);
            }
            threadWork(img, scene, rpp, subdivisions);

            for(int i = 0; i < nThreads; i++){
                pool[i].join();
            }
        }
        else{
            threadWork(img, scene, rpp, subdivisions);
        }
        
        //applyToSubimage(img, scene, rpp, subdivisions, 0);
        cout << "\r 100.00%" << endl;
    }
    
}

void PathTracer::threadWork(Image& img, Scene scene, int rpp, int subdivisions){
    while(pixelsLeftToGenerate > 0){
        applyToSubimage(img, scene, rpp, subdivisions);
    }
}

void PathTracer::applyToSubimage(Image& img, Scene scene, int rpp, int subdivisions){
    int w0, h0, wf, hf;
    const int width = scene.getWidth();
    const int height = scene.getHeight();
    

    getSubdivision(width, height, subdivisions, w0, wf, h0, hf);
    /*w0 = 270;
    wf = 274;
    h0 = 389;
    hf = 393;*/
    Direction d;
    Ray ray;
    vector<RGB> rayResult(rpp);
    Camera camera = scene.getCamera();
    Point o = camera.getOrigin();

    for(int i = w0; i < wf; i++){
        for(int j = h0; j < hf; j++){
            for(int r = 0; r < rpp; r++){
                /*Set d to a point of the projection plane (between -1 and 1)
                    *  The projection plane maps the image:
                    *      image[i, j] = projectionPlane[x, y] 
                    *
                    *      Relationship?
                    *          (x, y) = (i*a + b, j*c + d)   => We must find a, b, c, d
                    * 
                    *      nx and ny are the dimensions of the image
                    *      image[0, 0] = projectionPlane[-1, 1]
                    *          => (-1, 1) = (b, d)
                    *          => b = -1; d = 1
                    * 
                    * 
                    *      image[nx, ny] = projectionPlane[1, -1]  => (1, -1) = (nx*a -1, ny*c +1)
                    * 
                    *      Therefore:
                    *          nx*a -1 = 1
                    *              => nx*a = 2
                    *              => a = 2/nx
                    * 
                    *          ny*c +1 = -1
                    *              => ny*c = -2
                    *              => c = -2/ny
                    * 
                    *          x =  i*(2/nx) -1
                    *          y =  -j*(2/ny) +1
                    * 
                    *          image[i, j] = projectionPlane[2i/nx -1, -2j/ny +1]
                    *          we will also add a random factor for antialias            
                    */
                d.setAll((2*i + rng.getNumber(0,1))/width - 1 , (-2*j + rng.getNumber(0,1))/height +1 , 1);
                d.normalize();
                //Todo: mirar si es matriz inversa
                d = camera.changeToGlobalCoordinates(d);

                ray.setOrigin(o);
                ray.setDirection(d);    //Set the new direction as the ray's direction
                rayResult[r] = ray.getRayResult(scene); //Take RGB component found by the ray

                
            }
            img.setTuple(calculateRGBMean(rayResult), i, j);
        }
    }
    cout << "\r " << 100 - (pixelsLeftToGenerate * 100.0 / (width * height))  << "%" << flush;
}

int PathTracer::numberOfSubdivisions(int width, int height){
    return round(sqrt(sqrt(width*height)));
}

void PathTracer::getSubdivision(const int w, const int h, const int n, int& w0, int& wf, int& h0, int& hf){
    const int stepw = w/n, steph = h/n;
    unique_lock<mutex> lck(mtx);
    while(!ready){
        cv.wait(lck);
    }
    
    //w0 = the current cursor
    //wf = the current cursor + the size of the subdivision.
    //In case such position is out of bounds, it will be reset to the maximum width
    //Resets the current cursor to the next position that has to be taken
    //same with the height cursors
    w0 = current_width;
    wf = min(w, w0 + stepw);
    current_width = (wf) % w;

    h0 = current_height;
    hf = min(h, h0 + steph);
    current_height += steph * floor((wf) / w);

    pixelsLeftToGenerate -= (wf - w0) * (hf - h0);

    cv.notify_all();
}