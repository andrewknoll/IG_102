#include "LightPoint.hpp"

//***********************************************************************
// Class constructor
// @param localization Sets the localization of the light point
// @param emission RGB tuple of the light's emission
//***********************************************************************
LightPoint::LightPoint(Point localization, RGB emission){
    this->emission = emission;
    this->localization = localization;
}

//***********************************************************************
// Sets the value of the emission
// @param emission New RGB tuple of the light's emission
//***********************************************************************
void LightPoint::setEmission(RGB emission){
    this->emission = emission;
}

//***********************************************************************
// Returns light's emission
// @returns RGB tuple of the light's emission
//***********************************************************************
RGB LightPoint::getEmission(){
    return emission;
}

//***********************************************************************
// Sets the localization
// @param localization New localization for the light point
//***********************************************************************
void LightPoint::setLocalization(Point localization){
    this->localization = localization;
}

//***********************************************************************
// Returns the localization
// @returns Point where the light point is localized
//***********************************************************************
Point LightPoint::getLocalization(){
    return localization;
}