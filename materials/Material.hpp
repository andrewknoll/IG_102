#ifndef __MATERIAL__
#define __MATERIAL__

#include <math.h>
#include "../color/RGB.hpp"
#include "../rng.hpp"
#include "../vfield/Operations.hpp"

#define N_EVENTS 4

enum MaterialType{
    NONE, LIGHTSOURCE, DIFFUSE, PLASTIC, DIELECTRIC
};

enum Event : int{
    NO_EVENT=-1, ABSORPTION=0, DIFFUSION=1, SPECULAR=2, REFRACTION=3, LIGHTFOUND=4
};

Event& operator++(Event& e, int a);


class Material{

    private:
        RGB emission;
        RGB lambertian_coefficient;
        RGB specular_coefficient;
        RGB refraction_coefficient;
        MaterialType type;

        //cumulative probability of events
        double eP[N_EVENTS] = {1, 1, 1, 1};

        Event russianRoulette(bool firstTime);
        double getProb(Event e);

    public:
        Material();

        void setAsLightSource(RGB emission);
        void setAsDiffuse(RGB kl);
        void setAsPlastic(RGB kl, RGB ks);
        void setAsDielectric(RGB ks, RGB kt);
        
        RGB getEmission();
        MaterialType getMaterialType();
        
        bool is(MaterialType type);

        Event calculateRayCollision(RGB& factor, RGB& indirectLight, Direction rayDirection, Direction& newDirection, Point collisionPoint, Direction surfaceNormal, Direction tangent1, Direction tangent2, bool& init);

        RGB getCoefficient(Event e);

        RGB render();

};

#endif