#include "Scene.hpp"
#include "Images/Image.hpp"
#include "Color/RGB.hpp"
#include "Shapes/Shape.hpp"
#include "vfield/Point.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Operations.hpp"
#include <random>
#include <vector>

using namespace std;

struct randomNumberGen{
    default_random_engine re;
    uniform_real_distribution<double> unif{0, 1};

    double getNumber(){
        return unif(re);
    }
};

void rayTrace(Scene scene, int rpp){
    Image img;
    int width = scene.getWidth();
    int height = scene.getHeight();

    randomNumberGen rng;

    Direction d;

    PossibleSolution<Point> ps;
    Shape currentShape;
    Point o = scene.getCamera().getOrigin();

    vector<RGB> rayResult(rpp);

    if(scene.length() > 0){
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                for(int r = 0; r < rpp; r++){
                    d.setX(i + rng.getNumber());
                    d.setY(j + rng.getNumber());
                    d.setZ(1);

                    currentShape = scene.first();
                    ps = currentShape.findFirstIntersectionWithLine(d, o);

                    while(!ps.doesExist() && scene.hasNext()){
                        currentShape = scene.next();
                        ps = currentShape.findFirstIntersectionWithLine(d, o);
                    }

                    if(ps.doesExist()){
                        rayResult[r] = currentShape.getEmission();
                    }
                    else{
                        rayResult[r] = RGB(0, 0, 0);
                    }
                }
                img.setTuple(calculateRGBMean(rayResult), i, j);
            }
        }
    }
}