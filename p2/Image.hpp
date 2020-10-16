#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "RGB.hpp"

using namespace std;

class Image{
    private:
        string format;
        int width, height;
        float max = 1.0; 
        int colorRes;

        vector<vector<RGB>> m;

        float parseComment(string s);
        float diskToMemory(int i);
        int memoryToDisk(float f);

    public:
        void read(string filename);
        void toString();


};


#endif