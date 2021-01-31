#ifndef __LIGHTPOINT__
#define __LIGHTPOINT__
#include "color/RGB.hpp"
#include "vfield/Point.hpp"

//***********************************************************************
// Light point implementation
//***********************************************************************
class LightPoint{
    private:
        RGB emission;
        Point localization;

    public:
        LightPoint(Point localization, RGB emission);
        void setEmission(RGB emission);
        RGB getEmission();

        void setLocalization(Point localization);
        Point getLocalization();


};
#endif