#include "Shape.hpp"

long Shape::nextID = 0;

//***********************************************************************
// Returns the shape's unique ID
// @returns This shape's ID
//***********************************************************************
long Shape::getID(){
    return myID;
}

//***********************************************************************
// Returns the shape's material
// @returns This shape's material
//***********************************************************************
Material Shape::getMaterial(){
    return this->material;
}

//***********************************************************************
// Sets the shape's material
// @param material New material to be set
//***********************************************************************
void Shape::setMaterial(Material material){
    this->material = material;
};

//***********************************************************************
// Checks if two shapes are the same
// @param p1 Smart pointer to the first shape
// @param p2 Smart pointer to the second shape
// @returns true if and only if the shapes are the same
//***********************************************************************
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