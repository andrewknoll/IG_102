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

//Color resolution to be used in LDR format
#define LDR_RES 255

#include "../color/RGB.hpp"
#include "../color/ToneMapper.hpp"

using namespace std;
/**************************************************************
 Image representation class
 **************************************************************/
class Image{
    private:
        //Format of the output file
        string format;
        
        //Size of the image components
        int width, height;

        //Maximum value to translate LDR into HDR
        float max = 1.0; 

        //Color resolution to translate LDR into HDR
        int colorRes;
        
        //Color resolution of the output file
        int destinationCR = LDR_RES;

        float maxFound = 0.0;           //Maximum RGB value found in the image
        float maxAfterMapping = 0.0;    //Maximum RGB value after mapping the image
        float valuesMean = 0.0;         //Mean of the RGB values which have been read from LDR

        //Matrix of RGB values for the pixels
        vector<vector<RGB> > m;

        float parseComment(string s);
        float diskToMemory(int i);

        //Tone mapper to be applied
        shared_ptr<ToneMapper> tm;

    public:
        void setWidthHeight(int width, int height);
        int memoryToDisk(float f);
        string getFormat();
        int getWidth();
        int getHeight();
        float getMax();
        float getMean();

        void setMaxFound(float n);
        float getMaxFound();

        int getColorRes();
        void setDestinationResolution(int cr);

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