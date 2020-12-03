#include "Material.hpp"

Material::Material(){
    type = NONE;

}

Event& operator++(Event& e, int a){
    return e = (Event)((int)e + 1);
}

Event Material::russianRoulette(){
    double xi = rng.getNumber(0, 1);
    Event e = ABSORTION;
    while(e < N_EVENTS){
        //if the random number falls in the range of event e, we return that event e
        if(xi < eP[e]) break;
        e++;
    }
    return e;
}

double Material::getProb(Event e){
    double p;
    double maxK[3] = {
        lambertian_coefficient.max(),
        specular_coefficient.max(),
        refraction_coefficient.max()
    };
    if(e == ABSORTION){
        p = 0.1;
    }
    else{
        p = maxK[e-1] / (maxK[0] + maxK[1] + maxK[2] + 0.1);
    }
    return p;
}

void Material::setAsLightSource(RGB emission){
    this->emission = emission;
    lambertian_coefficient.reset();
    specular_coefficient.reset();
    refraction_coefficient.reset();
    type = LIGHTSOURCE;
}

void Material::setAsDiffuse(RGB kl){
    emission.reset();
    lambertian_coefficient = kl;
    specular_coefficient.reset();
    refraction_coefficient.reset();
    type = DIFFUSE;

    eP[ABSORTION] = 0.1;
    eP[DIFFUSE] = 1;

}

void Material::setAsPlastic(RGB kl, RGB ks){
    emission.reset();
    lambertian_coefficient = kl;
    specular_coefficient = ks;
    refraction_coefficient.reset();
    type = PLASTIC;

    eP[ABSORTION] = 0.1;
    eP[DIFFUSE] = 0.1 + getProb(DIFFUSION);
    eP[SPECULAR] = 1;
}

void Material::setAsDielectric(RGB ks, RGB kt){
    emission.reset();
    lambertian_coefficient.reset();
    specular_coefficient = ks;
    refraction_coefficient = kt;
    type = DIELECTRIC;

    eP[ABSORTION] = 0.1;
    eP[DIFFUSE] = 0.1;
    eP[SPECULAR] = 0.1 + getProb(SPECULAR);
    eP[REFRACTION] = 1;
}

RGB Material::getEmission(){
    return emission;
}

MaterialType Material::getMaterialType(){
    return type;
}

bool Material::is(MaterialType type){
    return this->type == type;
}

bool Material::calculateRayCollision(RGB& initial, Direction rayDirection, Direction& newDirection, Point collisionPoint, Direction surfaceNormal, Direction tangent1, Direction tangent2){
    double theta, phi;
    bool finished = false;
    if(type == LIGHTSOURCE){
        initial = initial * emission;
        finished = true;
    }
    else{
        Event e = russianRoulette();

        initial = initial * lambertian_coefficient / getProb(e);

        switch(e){
            case DIFFUSION:
                getAnglesByCosineSampling(theta, phi);
                newDirection = Direction(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
                break;
            case SPECULAR:
                newDirection = 2 * (rayDirection * surfaceNormal) * surfaceNormal - rayDirection;
                break;
            case REFRACTION:{
                float n;
                float c = -surfaceNormal * rayDirection;
                //if it's going inwards. We assume all transitions go from air to other media or viceversa
                if(surfaceNormal * rayDirection>0){  
                    n = 1 / refraction_coefficient.max();
                }
                else{
                    n = refraction_coefficient.max();
                }
                newDirection = rayDirection * n + surfaceNormal * (n * (c) * sqrt(1 - n*n*(1-c*c)));
            }
                break;
            default:
                finished = true;
        }

        newDirection = baseChange(newDirection, collisionPoint, tangent1, tangent2, surfaceNormal);
    }
    return finished;
}
