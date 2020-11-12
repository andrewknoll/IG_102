#ifndef __IMAGE__
#define __IMAGE__

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#define LDR_RES 255

#include "RGB.hpp"
#include "ToneMapper.hpp"

using namespace std;

class Image{
    private:
        string format;
        int width, height;
        float max = 1.0; 
        int colorRes;

        float maxFound = 0.0;
        float maxAfterMapping = 0.0;

        vector<vector<RGB> > m;

        float parseComment(string s);
        float diskToMemory(int i);

        shared_ptr<ToneMapper> tm;

    public:

        int memoryToDisk(float f);
        string getFormat();
        int getWidth();
        int getHeight();
        float getMax();

        void setMaxFound(float n);
        float getMaxFound();

        int getColorRes();

        void setMaxAfterMapping(float n);
        float getMaxAfterMapping();

        void setToneMapper(shared_ptr<ToneMapper> tm);
        void applyToneMapper();

        RGB getTuple(int i, int j);
        void setTuple(RGB color, int i, int j);

        void read(string filename);
        void toString();


};


#endif