/*
 * Algorithm obtained from paper (Retrieved 10 December 2020)
 * http://www.cmap.polytechnique.fr/~peyre/cours/x2005signal/hdr_photographic.pdf
 */
#include "Reinhard2002.hpp"

//***********************************************************************
//  Applies the Reinhard tone mapping technique (2002).
//  @param f Value to be mapped
//  @returns the mapped value
//***********************************************************************
float Reinhard2002::mapValue(float f){
    return f / (1 + f);
}