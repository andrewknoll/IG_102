#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>

#include "RGB.hpp"
#include "Image.hpp"

#define COMMENT '#'
#define MAX_COMMENT "#MAX="

using namespace std;

float loadColour(int colour, float max, int colorRes){
    return colour * max / colorRes;
}

float saveColour(float colour, float max, int colorRes){
    return colour * colorRes / max;
}


int main(){

    string filename;

    cout << "Image file: " << endl;
    cin >> filename;

    Image img;
    img.read(filename);

    img.toString();
    



    return 0;
}

