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
    private:
        float inclination;
        float azimuth;
        Planet* planet;
        Point UCSposition;
        Direction surfaceNormal;    // k vector(3) of coordinate system
        Direction longitudeTn;      // i vector(1) of coordinate system
        Direction latitudeTn;       // j vector(2) of coordinate system

        void calculateUCS(){
            // P(theta, phi) = c + r*(sin theta * sin phi, sin theta * cos phi, cos theta)
            float x = sin(inclination) * sin(azimuth) * planet->radius() + planet->center.getX();
            float y = sin(inclination) * cos(azimuth) * planet->radius() + planet->center.getY();
            float z = cos(inclination) * planet->radius() + planet->center.getZ();
            
            UCSposition.setAll(x, y, z);
        }

        void calculateSN(){
            // Partial derivative of the implicit sphere equation with respect to the station coordinates
            //                      station postion(UCS) - planet's center
            surfaceNormal.setX(UCSposition.getX()-planet->center.getX());
            surfaceNormal.setY(UCSposition.getY()-planet->center.getY());
            surfaceNormal.setZ(UCSposition.getZ()-planet->center.getZ());

            surfaceNormal.normalize();
        }

        void calculateTangents() {
            // longitude tangent: perpendicular to the planet's axis, follows the positive
            //      variation of the azimuth
            // latitude tangent: perpendicular to the other tangent, follows the negative 
            //      variation of the inclination
            // longitudeTn x latitudTn = surfaceNormal

            longitudeTn = cross(surfaceNormal, planet->axis);

            longitudeTn.normalize();

            latitudeTn = cross(longitudeTn, surfaceNormal);

            latitudeTn.normalize();
        }

    public:
        void setPlanet(Planet& p){
            planet = &p;
        }

        bool setInclination(float i){
            bool res = true;
            if(i > 0 && i < M_PI){
                inclination = i;
            }
            else{
                cerr << "ERROR: Inclination not in range (0,pi)" << endl;
                res = false;
            }
            return res;
        }

        bool setAzimuth(float a){
            bool res = true;
            if(a > -M_PI && a <= M_PI){
                azimuth = a;
            }
            else{
                cerr << "ERROR: Azimuth not in range (-pi,pi]" << endl;
                res = false;
            }
            return res;
        }

        void init(Planet& p, float inclination, float azimuth){
            setPlanet(p);
            if(setInclination(inclination)){
                if(setAzimuth(azimuth)){
                    calculateUCS();
                    calculateSN();
                    calculateTangents();
                }
            }
        }

        Point getUCS(){
            return UCSposition;
        }

        Planet* getPlanet(){
            return planet;
        }

        Direction getNormal(){
            return surfaceNormal;
        }

        Direction getLongT(){
            return longitudeTn;
        }

        Direction getLatiT(){
            return latitudeTn;
        }

        template <class T>
        T toLocal(T p){
            return baseChange(p, UCSposition, longitudeTn, latitudeTn, surfaceNormal);
        }

};





int main(){
    float v[9];

    for(int i = 0; i < 9; i++){
        cin >> v[i];
        cout << "He cogido v[" << i << "]" << endl;
    }

    Planet p;
    p.setCoords(v);

    float inc, azi;

    cout << "Inclination (degrees): " << flush;
    cin >> inc;
    inc = inc * M_PI /180;
    cout << inc;

    cout << "Azimuth (degrees): " << flush;
    cin >> azi;
    azi = azi * M_PI /180;
    cout << azi;

    PlanetaryStation ps;
    ps.init(p, inc, azi);

    cout << "UCS: " << ps.getUCS() << endl;
    cout << "Normal: " << ps.getNormal() << endl;
    cout << "Longitude: " << ps.getLongT() << endl;
    cout << "Latitude: " << ps.getLatiT() << endl;

     for(int i = 0; i < 9; i++){
        cin >> v[i];
        cout << "He cogido v[" << i << "]" << endl;
    }

    Planet p2;
    p2.setCoords(v);

    float inc2, azi2;

    cout << "Inclination (degrees): " << flush;
    cin >> inc2;
    inc2 = inc2 * M_PI /180;

    cout << "Azimuth (degrees): " << flush;
    cin >> azi2;
    azi2 = azi2 * M_PI /180;

    PlanetaryStation ps2;
    ps2.init(p2, inc2, azi2);

    cout << "UCS: " << ps2.getUCS() << endl;
    cout << "Normal: " << ps2.getNormal() << endl;
    cout << "Longitude: " << ps2.getLongT() << endl;
    cout << "Latitude: " << ps2.getLatiT() << endl;

    Direction distance = ps2.getUCS() - ps.getUCS();

    cout << "Direccion : " << distance << endl;
    
    Direction localP1 = ps.toLocal(distance);
    Direction localP2 = ps2.toLocal(distance);

    cout << "In Local system of P1: " << localP1 << endl;
    cout << "In Local system of P2: " << localP2 << endl;

    if(localP1.getZ() < 0){
        cerr << "WARNING: Trajectory of the matter traverses origin planet." << endl;
    }
    if(localP1.getZ() < 0){
        cerr << "WARNING: Trajectory of the matter traverses destination planet." << endl;
    }

    return 0;
}