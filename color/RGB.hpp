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
        void reset();
        float max();
        float operator[](const int i) const;
        float& operator[](const int i);
        RGB operator+(RGB b);
        RGB operator-(RGB b);
        RGB operator*(RGB b);
        RGB operator*(float b);
        RGB operator/(float b);
        RGB operator/(RGB b);
};

RGB calculateRGBMean(vector<RGB> v);

#endif