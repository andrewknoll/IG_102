#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Point.h"
#include "Direction.h"

#define THRESHOLD 1.0e-6

using namespace std;

struct Planet{
    Direction axis;
    Point center;
    Point refCity;

    Planet(float coord[9]){
        axis.setAll(coord[0], coord[1], coord[2]);
        center.setAll(coord[3], coord[4], coord[5]);
        refCity.setAll(coord[6], coord[7], coord[8]);

        Direction distance = refCity - center;

        if(abs(radius() - distance.modulus()) > THRESHOLD){
            cerr << "ERROR: Could not create planet." << endl;
            cerr << "The error between the radius and the distance between the center 
                     and the reference city is larger than the threshold (" << THRESHOLD << ")" << endl;
            exit(2);
        }
    }

    float radius(){
        return axis.modulus() / 2;
    }

};


int main(){
    float v[9];

    for(int i = 0; i < 9; i++){
        cin >> v[i];
        cout << "He cogido v[" << i << "]" << endl;
    }
    Planet p(v);

    cout << "Yay" << endl;
}