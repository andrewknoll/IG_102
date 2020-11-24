#include "Shape.hpp"

RGB Shape::getEmission(){
    return this->emission;
}

void Shape::setEmission(RGB emission){
    this->emission = emission;
};