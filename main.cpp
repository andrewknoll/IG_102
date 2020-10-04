#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "Point.hpp"
#include "Direction.hpp"
#include "Operations.hpp"

#define THRESHOLD 1.0e-6

using namespace std;

struct Planet{
    Direction axis;
    Point center;
    Point refCity;

    void setCoords(float coord[9]){
        axis.setAll(coord[0], coord[1], coord[2]);
        center.setAll(coord[3], coord[4], coord[5]);
        refCity.setAll(coord[6], coord[7], coord[8]);

        Direction distance = refCity - center;

        if(abs(radius() - distance.modulus()) > THRESHOLD){
            cerr << "ERROR: Could not create planet." << endl;
            cerr << "The error between the radius and the distance between the center and the reference city is larger than the threshold (" << THRESHOLD << ")" << endl;
            exit(2);
        }
    }

    float radius(){
        return axis.modulus() / 2;
    }
};

struct PlanetaryStation{
    float inclination;
    float azimuth;
    Planet* planet;

    void setPlanet(Planet& p){
        planet = &p;
    }

    void setInclination(float i){
        if(i > 0 && i < M_PI){
            inclination = i;
        }
        else{
            cerr << "ERROR: Inclination not in range (0,pi)" << endl;
        }
    }

    void setAzimuth(float a){
        if(a > -M_PI && a <= M_PI){
            azimuth = a;
        }
        else{
            cerr << "ERROR: Inclination not in range (-pi,pi]" << endl;
        }
    }
};


int main(){
    

    return 0;
}