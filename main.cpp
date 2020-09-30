#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "Point.h"
#include "Direction.h"
#include "Operations.h"

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
    float v[15];

    for(int i = 0; i < 15; i++){
        if(i == 0) cout << "Original point / direction:" << endl;
        else if(i == 3){
            
            cout << "Axis u:" << endl;
        }
        else if(i == 6){
            
            cout << "Axis v:" << endl;
        }
        else if(i == 9){
            
            cout << "Axis w:" << endl;
        }
        else if(i == 12){
            
            cout << "Origin point:" << endl;
        }
        cin >> v[i];
        cout << "He cogido v[" << i << "]" << endl;
    }
    Point original(v[0], v[1], v[2]);
    Direction ax_u(v[3], v[4], v[5]);
    Direction ax_v(v[6], v[7], v[8]);
    Direction ax_w(v[9], v[10], v[11]);
    Point o(v[12], v[13], v[14]);

    Coordinates result = baseChange(original, o, ax_u, ax_v, ax_w);


    cout << result << endl;

    return 0;
}