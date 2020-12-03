#include "Shape.hpp"

Material Shape::getMaterial(){
    return this->material;
}

void Shape::setMaterial(Material material){
    this->material = material;
};