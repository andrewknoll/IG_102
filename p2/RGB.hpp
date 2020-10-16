#ifndef RGBCLASS
#define RGBCLASS

#include <string>
#include <iostream>

using namespace std;

class RGB{
    private:
        float red, green, blue;

    public:
        RGB();
        RGB(float red, float green, float blue);
        void toString();
};

#endif