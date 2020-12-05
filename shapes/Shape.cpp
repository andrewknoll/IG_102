#include "Shape.hpp"

long Shape::nextID = 0;

Material Shape::getMaterial(){
    return this->material;
}

void Shape::setMaterial(Material material){
    this->material = material;
};

bool areEqual(shared_ptr<Shape> p1, shared_ptr<Shape> p2){
    bool equal;
    if(p1 != nullptr && p2 != nullptr){
        equal = p1->myID == p2->myID;
    }
    else{
        equal = (p1 == nullptr) != (p2 == nullptr);
    }
    return equal;
}