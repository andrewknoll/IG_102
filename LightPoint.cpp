#include "LightPoint.hpp"

LightPoint::LightPoint(Point localization, RGB emission){
    this->emission = emission;
    this->localization = localization;
}

void LightPoint::setEmission(RGB emission){
    this->emission = emission;
}

RGB LightPoint::getEmission(){
    return emission;
}

void LightPoint::setLocalization(Point localization){
    this->localization = localization;
}

Point LightPoint::getLocalization(){
    return localization;
}