#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "Image.hpp"

#define COMMENT '#'
#define MAX_COMMENT "#MAX="

using namespace std;

#ifdef CLAMP
float clamp(float val){
    float res = val;
    if(val > 1){
        res = 1;
    }
    return res;
}
#endif


#if defined(EQUALIZE) || defined(EQUALIZECLAMP) || defined(GAMMA) || defined(CLAMPGAMMA)
float equalize(float max, float val){
    return val / max;
}
#endif

#ifdef EQUALIZECLAMP
float equalizeAndClamp(float max, float V, float val){
    float res = val;
    if(val > V){
        res = V;
    }
    else{
        res = equalize(max, val);
    }
    return res;
}
#endif

#if defined(GAMMA) || defined(CLAMPGAMMA)
float gamma(float max, float g, float val){
    float res = equalize(max, val);
    return pow(res, g);
}
#endif

#ifdef CLAMPGAMMA
float clampAndGamma(float max, float V, float g, float val){
    float res = val;
    if(val > V){
        res = V;
    }
    else{
        gamma(max, g, val);
    }
    return res;
}
#endif

float map(float max, float V, float g, float val){
    #ifdef CLAMP
        return clamp(val);
    #endif

    #ifdef EQUALIZE
        return equalize(max, val);
    #endif

    #ifdef EQUALIZECLAMP
        return equalizeAndClamp(max, V, val);
    #endif

    #ifdef GAMMA
        return gamma(max, g, val);
    #endif

    #ifdef CLAMPGAMMA
        return clampAndGamma(max, V, g, val);
    #endif
}

void generateOutput(Image& img, string output, float max, float V, float gamma){

    ofstream fout(output);

    if(fout.is_open()){

        int width = img.getWidth();
        int height = img.getHeight();
        float max = img.getMax();

        fout << img.getFormat() << endl << MAX_COMMENT << max << endl << width << " " << height << endl << img.getColorRes() << endl;
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){

                float red = img.getTuple(i, j).get(0);
                float green = img.getTuple(i, j).get(1);
                float blue = img.getTuple(i, j).get(2);

                fout << map(max, V, gamma, red) << " " << map(max, V, gamma, green) << " " << map(max, V, gamma, blue);
                fout << "    " << endl;
            }
        }
    }
    else{
        cerr << "Couldn't write on " << output << endl;
    }

    fout.close();
}

int main(int argc, char** argv){

    string input, output;

    if(argc >= 3){
        input = argv[1];
        output = argv[2];

        float max = -1, gamma = -1, V = -1;

        #ifdef EQUALIZECLAMP
            if(argc >= 4){
                V = atof(argv[3]);
            }
            else{
                cerr << "V argument not specified" << endl;
                exit(1);
            }
        #endif

        #ifdef GAMMA
            if(argc >= 4){
                gamma = atof(argv[3]);
            }
            else{
                cerr << "GAMMA argument not specified" << endl;
                exit(2);
            }
        #endif

        #ifdef CLAMPGAMMA
            if(argc >= 5){
                V = atof(argv[3]);
                gamma = atof(argv[4]);
                
            }
            else{
                cerr << "Insuficient arguments (need V and gamma)" << endl;
                exit(3);
            }
        #endif

        Image img;
        img.read(input);

        #if defined(EQUALIZE) || defined(EQUALIZECLAMP) || defined(GAMMA) || defined(CLAMPGAMMA)
            max = img.getMax();
        #endif

        generateOutput(img, output, max, V, gamma);
    }
    else{
        cerr << "Input or output arguments not specified" << endl;
        exit(1);
    }
    return 0;
}

