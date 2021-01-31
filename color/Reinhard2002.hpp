/*
 * Algorithm obtained from paper (Retrieved 10 December 2020)
 * http://www.cmap.polytechnique.fr/~peyre/cours/x2005signal/hdr_photographic.pdf
 *
 */
#ifndef __REINHARD2002__
#define __REINHARD2002__

#include "ToneMapper.hpp"
/**************************************************************
 Reinhard tone mapping technique (2002) class
 **************************************************************/
class Reinhard2002: public ToneMapper{
    public:

        float mapValue(float f);

};


#endif