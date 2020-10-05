#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    Point UCSposition;
    Direction surfaceNormal;    // k vector(3) of coordinate system
    Direction longitudeTn;      // i vector(1) of coordinate system
    Direction latitudeTn;       // j vector(2) of coordinate system

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

    void calculateUCS(){
        // P(theta, phi) = c + r*(sin theta * sin phi, sin theta * cos phi, cos theta)
        float x = sin(inclination) * sin(azimuth) * planet->radius() + planet->center.getX();
        float y = sin(inclination) * cos(azimuth) * planet->radius() + planet->center.getY();
        float z = cos(inclination) * planet->radius() + planet->center.getZ();
        
        UCSposition.setAll(x, y, z);
    }

    void calculateSN(){
        // Derivation of the sphere function in inclination and azimuth -> 
        //                      station postion(UCS) - planet's center
        surfaceNormal.setX(UCSposition.getX()-planet->center.getX());
        surfaceNormal.setY(UCSposition.getY()-planet->center.getY());
        surfaceNormal.setZ(UCSposition.getZ()-planet->center.getZ());
    }

    void calculateTangents() {
        // longitude tangent: perpendicular to the planet's axis, follows the positive
        //      variation of the azimuth
        // latitude tangent: perpendicular to the other tangent, follows the negative 
        //      variation of the inclination
        // longitudeTn x latitudTn = surfaceNormal
        float longitudeX = planet->center.getX() + planet->radius() * cos(inclination) * sin(azimuth);
        float longitudeY = planet->center.getY() + planet->radius() * cos(inclination) * cos(azimuth);
        float longitudeZ = planet->center.getZ() + planet->radius() * (-sin(inclination));
        longitudeTn.setAll(longitudeX, longitudeY, longitudeZ);

        float latitudeX = planet->center.getX() + planet->radius() * sin(inclination) * cos(azimuth);
        float latitudeY = planet->center.getY() + planet->radius() * sin(inclination) * (-sin(azimuth));
        float latitudeZ = planet->center.getZ() + planet->radius() * cos(inclination);
        latitudeTn.setAll(latitudeX, latitudeY, latitudeZ);
    }
};


int main(){
    PlanetaryStation ps1, ps2;
    float pl1[9], pl2[9];
    // Ask for planetario station parameters
    cout << "Introduzca el planeta de la estacion planetaria origen: " << endl;
    cout << "Eje: ";
    cin >> pl1[0] >> pl1[1] >> pl1[2];
    cout << ""

    return 0;
}