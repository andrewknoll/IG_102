#ifndef RGBCLASS
#define RGBCLASS

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class RGB{
    private:
        float red, green, blue;

    public:
        RGB();
        RGB(float red, float green, float blue);
        void toString();
        float get(int i);
        void set(float c, int i);
};

RGB calculateRGBMean(vector<RGB> v){
    float results[3];
    int size = v.size();
    for(int i = 0; i < size; i++){
        for(int c = 0; c < 3; c++){
            results[c] += v[i].get(c);
        }
    }
    return RGB(results[0] / size, results[1] / size, results[2] / size);
}

#endif