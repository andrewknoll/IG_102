#ifndef __MATERIAL__
#define __MATERIAL__

#include <math.h>
#include <algorithm>
#include <iterator>
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
        RGB refractive_index;

        MaterialType type;

        //cumulative probability of events
        float eP[N_EVENTS] = {1, 1, 1, 1};

        Event russianRoulette(float K[4], bool init);
        float getProb(Event e, RGB Pk[4]);
        void getCoefficientsArray(RGB maxK[3]);
        float applySnell(RGB n0, RGB n1, float incident_angle);
        void recalculateWithFresnel(RGB k[3], float Pk[4], RGB n0, RGB n1, float incident_angle, float refraction_angle);

    public:
        Material();

        void setAsLightSource(RGB emission);
        void setAsDiffuse(RGB kl);
        void setAsPlastic(RGB kl, RGB ks);
        void setAsDielectric(float absorptionProb, RGB n);
        void setAsCustomMaterial(RGB kl, RGB ks, RGB kt);
        
        RGB getEmission();
        MaterialType getMaterialType();
        
        bool is(MaterialType type);

        Event calculateRayCollision(RGB& factor, RGB& indirectLight, Direction rayDirection, Direction& newDirection, Point collisionPoint, Direction surfaceNormal, Direction tangent1, Direction tangent2, bool& init);

        RGB getCoefficient(Event e);

        RGB render();

};

#endif