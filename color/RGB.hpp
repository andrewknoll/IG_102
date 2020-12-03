#ifndef __RGB__
#define __RGB__

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
        void reset();
        float max();
        RGB operator*(RGB& b);
        RGB operator*(double b);
        RGB operator/(double b);
};

RGB calculateRGBMean(vector<RGB> v);

#endif