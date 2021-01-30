#include "Material.hpp"

Material::Material(){
    type = NONE;

}

Event& operator++(Event& e, int a){
    return e = (Event)((int)e + 1);
}

Event Material::russianRoulette(float K[4], bool init){
    float xi = rng.getNumber(0, 1);
    Event e = ABSORPTION;
    if(K[0] < 1 && !init){
        xi += K[0];
    }
    while(e < N_EVENTS){
        //if the random number falls in the range of event e, we return that event e
        if(xi < K[e]) break;
        e++;
    }

    return e;
}

float Material::getProb(Event e, RGB k[3]){
    float p;
    RGB coeff[3];
    if(k == nullptr){
        getCoefficientsArray(coeff);
        if(e == ABSORPTION){
            p = 1 - coeff[0].max() - coeff[1].max() - coeff[2].max();
        }
        else{
            p = coeff[e-1].max();
        }
    }
    else{
        if(e == ABSORPTION){
            p = 1 - k[0].max() - k[1].max() - k[2].max();
        }
        else{
            p = k[e-1].max();
        }
    }
    return p;
}
//eP components for ABSORPTION and DIFFUSION events must have been set previously
void Material::recalculateWithFresnel(RGB k[3], float Pk[4], RGB n0, RGB n1, float incident_angle, float refraction_angle){
    float cosine = cosf(incident_angle);
    //This would mean the normal is flipped with respect to where the ray is coming from
    //(most likely the ray is "inside" the shape)
    if(cosine < 0) cosine = -cosine;
    RGB t0 = n1 * cosine, t1 = n0*cosf(refraction_angle);
    RGB t2 = n0 * cosine, t3 = n1*cosf(refraction_angle);
    RGB par = (t0 - t1) / (t0 + t1), per = (t2 - t3) / (t2 + t3);

    float sine2 = (n0.max()/n1.max()) * (n0.max()/n1.max()) * (1- cosine * cosine);

    if(sine2 < 1){ 
        k[1] = (par*par + per*per) / 2;
        k[2] = RGB(1,1,1) - k[1];
    }
    else{ //Total Internal Reflection
        k[1] = RGB(1-eP[DIFFUSION],1-eP[DIFFUSION],1-eP[DIFFUSION]);
        k[2] = RGB(0,0,0);
    }

    Pk[ABSORPTION] = eP[ABSORPTION];
    Pk[DIFFUSION] = eP[DIFFUSION];
    Pk[SPECULAR] = Pk[DIFFUSION] + k[1].max();
    Pk[REFRACTION] = 1; 
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
    if(kl.max() >= 1){
        cerr << "WARNING: The max coefficients should be less than 1 for a correct behaviour." << endl;
    }
    lambertian_coefficient = kl;
    specular_coefficient.reset();
    refraction_coefficient.reset();
    type = DIFFUSE;

    
    eP[ABSORPTION] = 1 - getProb(DIFFUSION, nullptr);
    eP[DIFFUSION] = 1;

}

void Material::setAsPlastic(RGB kl, RGB ks){
    emission.reset();
    if(ks.max() + kl.max() >= 1){
        cerr << "WARNING: The sum of the max coefficients should be less than 1 for a correct behaviour." << endl;
    }
    lambertian_coefficient = kl;
    specular_coefficient = ks;
    refraction_coefficient.reset();
    type = PLASTIC;

    eP[ABSORPTION] = 1 - getProb(DIFFUSION, nullptr) - getProb(SPECULAR, nullptr);
    eP[DIFFUSION] = eP[ABSORPTION] + getProb(DIFFUSION, nullptr);
    eP[SPECULAR] = 1;
}

void Material::setAsDielectric(float absorptionProb, RGB n){
    emission.reset();
    //sum of ks and kt must be less than 1
    if(absorptionProb > 1){
        cerr << "WARNING: Absorption probability should be less than 1 for a correct behaviour." << endl;
    }

    lambertian_coefficient.reset();
    specular_coefficient.reset();
    refraction_coefficient.reset();
    refractive_index = n;
    type = DIELECTRIC;

    eP[ABSORPTION] = absorptionProb;
    eP[DIFFUSION] = eP[ABSORPTION];
    //The values for Reflection and Refraction will be set later by Fresnel Equations
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
    if(e == DIFFUSION){
        result = lambertian_coefficient;
    }
    else if(e == SPECULAR){
        result = specular_coefficient;
    }
    else if(e == REFRACTION){
        result = refraction_coefficient;
    }
    return result;
}

void Material::getCoefficientsArray(RGB maxK[3]){
    maxK[0] = lambertian_coefficient;
    maxK[1] = specular_coefficient;
    maxK[2] = refraction_coefficient;
}

float Material::applySnell(RGB n0, RGB n1, float incident_angle){
    //Returns the angle for the component with the highest coefficient
    return asinf(sinf(incident_angle) * n0.max()/n1.max());
}

Event Material::calculateRayCollision(RGB& factor, RGB& indirectLight, Direction rayDirection, Direction& newDirection, Point collisionPoint, Direction surfaceNormal, Direction tangent1, Direction tangent2, bool& init){
    float theta, phi, acc_k[4];
    RGB n0, n1;
    copy(begin(eP), end(eP), begin(acc_k));
    RGB k[3] = {RGB(0,0,0), RGB(0,0,0), RGB(0,0,0)};
    Event e = NO_EVENT;
    RGB operand;

    //For any pair of unitary vectors: cos(angle) = v1 * v2
    float cosine = surfaceNormal * rayDirection;

    if(type == LIGHTSOURCE){
        indirectLight = emission;
        e = LIGHTFOUND;
    }
    else{
        getCoefficientsArray(k);
        if(type == DIELECTRIC){
            float incident_angle;
            if(surfaceNormal * rayDirection<0){
                //Dot product is negative when the angle is in the second or third quadrant
                //Which means one of the components of the vectors is in the opposite sense
                //When the normal and the ray are going in opposite senses, it means it's going IN of the shape
                //Therefore the quotient of the refraction coefficients is 1 (air's) / n (shape's)
                n0 = RGB(1,1,1);
                n1 = refractive_index;
            }
            else{
                //we are going outwards, therefore, the quotient is n (shape's) / 1 (air's)
                n0 = refractive_index;
                n1 = RGB(1,1,1);
            }
            
            incident_angle = findAngle(rayDirection, surfaceNormal);
            recalculateWithFresnel(k, acc_k, n0, n1, incident_angle, applySnell(n0, n1, incident_angle));
        }
        e = russianRoulette(acc_k, init);
        if(e!=ABSORPTION){
            operand = k[e-1] / getProb(e, k);
            if(!init){
                factor = operand;
                init = true;
            } 
            else{
                factor = factor * operand;
            }
            if(cosine > 0){
                surfaceNormal = -surfaceNormal;
            }
        }

        switch(e){
            case DIFFUSION:
                //Normal * rayDirection < 0 so secondary rays are always casted in the opposite direction of rayDirection
                getAnglesByCosineSampling(theta, phi);
                newDirection = Direction(sinf(theta)*cosf(phi), sinf(theta)*sinf(phi), cosf(theta));
                newDirection = baseChange(newDirection, collisionPoint, tangent1, tangent2, surfaceNormal);
                break;
            case SPECULAR:
                newDirection = rayDirection - 2 * (rayDirection * surfaceNormal) * surfaceNormal;
                break;
            case REFRACTION:{
                float n = n0.max() / n1.max();
                if(cosine < 0){ //cosine must be positive
                    cosine = -cosine;
                }
                
                float sine2thetaT = n*n * (1- cosine * cosine);
                if(sine2thetaT > 1){    //Total Internal Reflection
                    newDirection = rayDirection - 2 * (rayDirection * surfaceNormal) * surfaceNormal;
                }
                else{
                    newDirection = rayDirection * n + surfaceNormal * (n*cosine - sqrtf(1 -sine2thetaT));
                }
                
            }
            break;
            
            default:
                break;
        }
        newDirection.normalize();
    }
    return e;
}
