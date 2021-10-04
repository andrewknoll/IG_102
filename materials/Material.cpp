#include "Material.hpp"

Material::Material(){
    type = NONE;

}
//***********************************************************************
// Postfix Operator++ for events. Modifies the value of e.
// @param e Event to be modified
// @param a Integer to be added (Ommited)
// @returns e++
//***********************************************************************
Event& operator++(Event& e, int a){
    return e = (Event)((int)e + 1);
}

//***********************************************************************
// Returns an event using the array of cumulative probabilities.
// @param K[] array of elements with the cumulative probabilities (in order) for each event
// @param init When set to true, the event will not be ABSORPTION, unless the probability of such event is 1
// @returns A random event with the probabilities indicated by K
//***********************************************************************
Event Material::russianRoulette(float K[], bool init){
    float xi = rng.getNumber(0, 1);
    Event e = ABSORPTION;
    if(K[0] < 1 && !init){
        xi += K[0];
        if(xi > 1){
            xi = 1;
        }
    }
    while(e < N_EVENTS - 1){
        //if the random number falls in the range of event e, we return that event e
        if(xi <= K[e]) break;
        e++;
    }

    return e;
}

//***********************************************************************
// Returns the probability of an event happening given an array of coefficients.
// @param k[3] array of the lambertian, specular and refraction coefficients of the material. When null, 
// the fields lambertian_coefficient, specular_coefficient and refraction_coefficient will be used.
// @param e The event
// @returns The probability of event e given the RGB coefficients
//***********************************************************************
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
//***********************************************************************
// Recalculates the specular and refraction coefficients following Fresnel Equations.
// @note eP components for ABSORPTION and DIFFUSION events must have been set previously
// @param k[3] array where the lambertian, specular and refraction coefficients of the material (recalculated
//      with Fresnel equations) will be stored
// @param Pk[] array where the cumulative probabilities (recalculated by Fresnel equations) 
//      of absorption, diffusion, specular reflection and refraction
//      events will be stored
// @param n0 Refractive Index (in RGB) of the first medium
// @param n1 Refractive Index (in RGB) of the second medium
// @param incident_angle Angle of incidence (between surface's normal and the ray)
// @param refraction_angle Angle of refraction, calculated by Snell's law
//***********************************************************************
void Material::recalculateWithFresnel(RGB k[3], float Pk[], RGB n0, RGB n1, float incident_angle, float refraction_angle){
    float cosine_in = cosf(incident_angle);
    float cosine_out = cosf(refraction_angle);
    //This would mean the normal is flipped with respect to where the ray is coming from
    //(most likely the ray is "inside" the shape)
    if(cosine_in < 0) cosine_in = -cosine_in;
    RGB t0 = n1 * cosine_in, t1 = n0*cosine_out;
    RGB t2 = n0 * cosine_in, t3 = n1*cosine_out;
    RGB par = (t0 - t1) / (t0 + t1), per = (t2 - t3) / (t2 + t3);

    if(cosine_out > EPSILON){ 
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
//***********************************************************************
// Sets the material as a Light Source (resets all the coefficient and assigns the emission RGB tuple)
// @param emission New emission to be had by the material
//***********************************************************************
void Material::setAsLightSource(RGB emission){
    this->emission = emission;
    lambertian_coefficient.reset();
    specular_coefficient.reset();
    refraction_coefficient.reset();
    type = LIGHTSOURCE;
}

//***********************************************************************
// Sets the material as a Lambertian material
// (resets the specular, refraction and emission coefficients)
// Recalculates cumulative probabilities
// Prints a warning if the parameter is higher than 1 (defies physics)
// @param kl New lambertian coefficient
//***********************************************************************
void Material::setAsLambertian(RGB kl){
    emission.reset();
    if(kl.max() >= 1){
        cerr << "WARNING: The max coefficients should be less than 1 for a correct behaviour." << endl;
    }
    lambertian_coefficient = kl;
    specular_coefficient.reset();
    refraction_coefficient.reset();
    type = LAMBERTIAN;

    
    eP[ABSORPTION] = 1 - getProb(DIFFUSION, nullptr);
    eP[DIFFUSION] = 1;

}

//***********************************************************************
// Sets the material as a Plastic material
// (resets the refraction and emission coefficients)
// Recalculates cumulative probabilities
// Prints a warning if the sum of the parameter is higher than 1 (defies physics)
// @param kl New lambertian coefficient
// @param ks New specular coefficient
//***********************************************************************
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

//***********************************************************************
// Sets the material as a Dielectric material (follows Fresnel Equations)
// (resets all coefficients)
// Recalculates the DIFFUSION cumulative probability
// Prints a warning if the absorption probability is higher than 1 (defies statistics)
// @param absorptionProb New absorption probability
// @param n New refractive index
//***********************************************************************
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

//***********************************************************************
// Sets the material as a Custom material (does not follow Fresnel Equations)
// (resets the emission coefficient)
// Recalculates the cumulative probabilities
// Prints a warning if the sum of the coefficients is higher than 1 (defies physics)
// @param kl New lambertian coefficient
// @param ks New specular coefficient
// @param kt New refraction coefficient
//***********************************************************************
void Material::setAsCustomMaterial(RGB kl, RGB ks, RGB kt){
    emission.reset();
    if(kl.max() + ks.max() + kt.max() >= 1){
        cerr << "WARNING: The sum of the max coefficients should be less than 1 for a correct behaviour." << endl;
    }
    lambertian_coefficient = kl;
    specular_coefficient = ks;
    refraction_coefficient = kt;
    type = CUSTOM;

    
    eP[ABSORPTION] = 1 - getProb(DIFFUSION, nullptr) - getProb(SPECULAR, nullptr);
    eP[DIFFUSION] = eP[ABSORPTION] + getProb(DIFFUSION, nullptr);
    eP[SPECULAR] = eP[DIFFUSION] + getProb(SPECULAR, nullptr);
    eP[REFRACTION] = 1;
    
}

//***********************************************************************
// @returns emission coefficient
//***********************************************************************
RGB Material::getEmission(){
    return emission;
}

//***********************************************************************
// @returns Material type
//***********************************************************************
MaterialType Material::getMaterialType(){
    return type;
}

//***********************************************************************
// @param type Material type to be checked
// @returns true if the material is of the specified type
//***********************************************************************
bool Material::is(MaterialType type){
    return this->type == type;
}

//***********************************************************************
// @param e Event
// @returns specified coefficient for event e
//***********************************************************************
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

//***********************************************************************
// Stores the lambertian, specular and refraction coefficients in the array
// @param maxK[3] array where result is going to be stored
//***********************************************************************
void Material::getCoefficientsArray(RGB maxK[3]){
    maxK[0] = lambertian_coefficient;
    maxK[1] = specular_coefficient;
    maxK[2] = refraction_coefficient;
}

//***********************************************************************
// Implementation of Snell's law
// @param n0 Refractive Index (in RGB) of the first medium
// @param n1 Refractive Index (in RGB) of the second medium
// @param incident_angle Angle of incidence (between surface's normal and the ray)
// @returns The angle of refraction, calculated by Snell's law
//***********************************************************************
float Material::applySnell(RGB n0, RGB n1, float incident_angle){
    //Returns the angle for the component with the highest coefficient
    return asinf(sinf(incident_angle) * n0.max()/n1.max());
}

//***********************************************************************
// Calculates the result of a collision of a ray with the material
// @param tp Stores the throughput of the path that the ray has followed.
//      will be modified to accomodate the event that has occured upon the collision
// @param il Will store the value of the emission of the material, if it is a light source 
// @param rd The direction of the incoming ray
// @param nd Will store the direction of the bounce the ray has to follow (if any)
// @param cp Stores the point where the ray collided with the shape made of this material
// @param normal Surface normal of the shape made of this material
// @param t1 A tangent to the surface of the shape made of this material
// @param t2 A tangent to the surface of the shape made of this material. Must be perpendicular to t1.
// @param init Stores false if and only if this is the first bounce of the ray. Will store true.
// @returns The event that happened on this bounce
//***********************************************************************
Event Material::calculateRayCollision(RGB& tp, RGB& il, Direction rd, Direction& nd, 
                                      Point cp, Direction normal, Direction t1,
                                    Direction t2, bool& init){
    //Variable initialization
    float theta, phi, acc_k[4];
    RGB n0, n1;
    copy(begin(eP), end(eP), begin(acc_k));
    RGB k[3] = {RGB(0,0,0), RGB(0,0,0), RGB(0,0,0)};
    Event e = NO_EVENT;
    RGB operand;

    //For any pair of unitary vectors: cos(angle) = v1 * v2
    float cosine = normal * rd;

    //Asssign indirect light if this is a light source
    if(type == LIGHTSOURCE){
        il = emission;
        e = LIGHTFOUND;
    }
    else{
        //Get the default coefficients
        getCoefficientsArray(k);

        //Apply Fresnel Equations
        if(type == DIELECTRIC){
            float incident_angle = findAngle(rd, normal);
            if(normal * rd<0){
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
            recalculateWithFresnel(k, acc_k, n0, n1, incident_angle, applySnell(n0, n1, incident_angle));
        }
        //Do russian roulette with Fresnel's results (if Fresnel was not applied, normal probabilities will be used)
        e = russianRoulette(acc_k, init);

        //Modify Throughput
        if(e!=ABSORPTION){
            operand = k[e-1] / getProb(e, k);
            if(!init){
                tp = operand;
                init = true;
            } 
            else{
                tp = tp * operand;
            }
        }

        //Calculate direction of the next bounce
        switch(e){
            case DIFFUSION: //Sampled by cosine sampling
                //Normal * rd < 0 so secondary rays are always casted in the opposite direction of rd
                if(cosine > 0){
                    normal = -normal;
                }
                getAnglesByCosineSampling(theta, phi);
                nd = Direction(sinf(theta)*cosf(phi), sinf(theta)*sinf(phi), cosf(theta));
                nd = baseChange(nd, t1, t2, normal, cp);
                break;
            case SPECULAR:  //Following reflection law
                if(cosine > 0){
                    normal = -normal;
                }
                nd = rd - 2 * (rd * normal) * normal;
                break;
            case REFRACTION:{   //Following Snell's law
                float n = n0.max() / n1.max();

                float sine2thetaT = n*n * (1- cosine * cosine);

                if(sine2thetaT > 1){    //Total Internal Reflection
                    if(cosine > 0){
                        normal = -normal;
                    }
                    nd = rd - 2 * (rd * normal) * normal;
                }
                else{
                    nd = rd * n + normal * (n*cosine - sqrtf(1 -sine2thetaT));
                }
                
            }
            break;
            
            default: //Nothing to be done for Absorption event
                break;
        }
        //Normalize the new direction
        nd.normalize();
    }
    return e;
}
