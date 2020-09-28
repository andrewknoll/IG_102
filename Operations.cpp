#include "Operations.h"

Coordinates baseChange(Coordinates& coord, const Point o, const Direction u, const Direction v, const Direction w){
    Matrix4 mat;
    mat.setColumn(0, u.getVector());
    mat.setColumn(1, v.getVector());
    mat.setColumn(2, w.getVector());
    mat.setColumn(3, o.getVector());

    return coord * mat;

}