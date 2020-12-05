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
    if(type==PLASTIC && e==REFRACTION){
        cout <<"UMNUMUMNUUMNUNUNUMNUNUNNNMUNMNUNMUNUNNUNMUNMNUNUNUNNUMNUNMUNMNUNMUNMUNMNMNUNMUNMUNMNUMNUNUNUUUU" << endl;
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
        p = 1 - maxK[0] - maxK[1] - maxK[2];
    }
    else{
        p = maxK[e-1];
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
    for(int i = 0; i < 3; i++){
        if(kl.get(i) > 0.9){
            kl.set(0.9, i);
        }
    }
    lambertian_coefficient = kl;
    specular_coefficient.reset();
    refraction_coefficient.reset();
    type = DIFFUSE;

    
    eP[ABSORTION] = 1 - getProb(DIFFUSION);
    eP[DIFFUSION] = 1;

}

void Material::setAsPlastic(RGB kl, RGB ks){
    emission.reset();
    double sum;
    for(int i = 0; i < 3; i++){
        sum = kl.get(i) + ks.get(i);
        //sum of kl and ks must be less than 0.9
        if(sum > 0.9){
            kl.set(kl.get(i)/sum * 0.9, i);
            ks.set(ks.get(i)/sum * 0.9, i);
        }
    }
    lambertian_coefficient = kl;
    specular_coefficient = ks;
    refraction_coefficient.reset();
    type = PLASTIC;

    eP[ABSORTION] = 1 - getProb(DIFFUSION) - getProb(SPECULAR);
    eP[DIFFUSION] = eP[ABSORTION] + getProb(DIFFUSION);
    eP[SPECULAR] = 1;
}

void Material::setAsDielectric(RGB ks, RGB kt){
    emission.reset();
    double sum;
    for(int i = 0; i < 3; i++){
        sum = ks.get(i) + kt.get(i);
        //sum of ks and kt must be less than 0.9
        if(sum > 0.9){
            ks.set(ks.get(i)/sum * 0.9, i);
            kt.set(kt.get(i)/sum * 0.9, i);
        }
    }
    lambertian_coefficient.reset();
    specular_coefficient = ks;
    refraction_coefficient = kt;
    type = DIELECTRIC;

    eP[ABSORTION] = 1 - getProb(SPECULAR) - getProb(REFRACTION);
    eP[DIFFUSION] = eP[ABSORTION];
    eP[SPECULAR] = eP[ABSORTION] + getProb(SPECULAR);
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

RGB Material::getCoefficient(Event e){
    RGB result;
    if(e==DIFFUSION){
        result = lambertian_coefficient;
    } 
    else if(e==SPECULAR){
        result = specular_coefficient;
    }
    else if(e==REFRACTION){
        result = refraction_coefficient;
    }
    return result;
}

Event Material::calculateRayCollision(RGB& initial, Direction rayDirection, Direction& newDirection, Point collisionPoint, Direction surfaceNormal, Direction tangent1, Direction tangent2, bool& init){
    double theta, phi;
    Event e = NO_EVENT;
    RGB factor;
    if(type == LIGHTSOURCE){
        if(!init){
            initial = emission;
            init = true;
        } 
        else{
            initial = initial * emission;
        }
    }
    else{
        e = russianRoulette();
        if(e!=ABSORTION){
            factor = getCoefficient(e) / getProb(e);
            if(!init){
                initial = factor;
                init = true;
            } 
            else{
                initial = initial * factor;
            }
        }

        switch(e){
            case DIFFUSION:
                getAnglesByCosineSampling(theta, phi);
                newDirection = Direction(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
                newDirection = baseChangeInverse(newDirection, collisionPoint, tangent1, tangent2, surfaceNormal);
                break;
            case SPECULAR:
                newDirection = rayDirection - 2 * (rayDirection * surfaceNormal) * surfaceNormal;
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
                break;
        }
        newDirection.normalize();
    }
    return e;
}
