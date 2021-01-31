#include "Matrix4.hpp"

//***********************************************************************
// Sets this matrix to an identity 4x4 matrix
//***********************************************************************
void Matrix4::identity(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            m[i][j] = (i==j)? 1 : 0;
        }
    }
}

//***********************************************************************
// Returns the determinant of a 3x3 matrix
// @param mat[3][3] 3x3 array
// @returns the determinant of the matrix formed by the values in mat
//***********************************************************************
float Matrix4::det3(float mat[3][3]){
    return mat[0][0] * mat[1][1] * mat[2][2] 
         + mat[1][0] * mat[2][1] * mat[0][2] 
         + mat[2][0] * mat[0][1] * mat[1][2] 
         - mat[0][2] * mat[1][1] * mat[2][0] 
         - mat[1][2] * mat[2][1] * mat[0][0]
         - mat[2][2] * mat[0][1] * mat[1][0];
}

//***********************************************************************
// Returns the determinant of this matrix. If it hadn't been previously calculated,
// it is calculated.
// @returns the determinant of this matrix
//***********************************************************************
float Matrix4::det(){
    float det = 0.0;
    if(detCalculated) det = determinant;
    else{ //Calculate determinant if it's not calculated
        //uses the first column as pivot
        float aux[3][3];
        for(int i = 0; i < 4; i++){
            removeRowColumn(i, 0, aux);
            if(i % 2 == 0){
                det += m[i][0] * det3(aux);
            }
            else{
                det -= m[i][0] * det3(aux);
            }
        }
        detCalculated = true;
        determinant = det;

    }
    return det;
}

//***********************************************************************
// Returns the inverse of this matrix, or throws an exception if the matrix has
// no inverse. Uses the adjunct matrix method.
// @throws NoInverseException if matrix has no exception
// @returns The inverse of this matrix
//***********************************************************************
Matrix4 Matrix4::inverse(){
    Matrix4 adjunct;
    float aux[3][3];

    float det = 0.0;

    if(detCalculated) {
        det = determinant;
        if(det == 0) throw NoInverseException(*this);
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            removeRowColumn(i, j, aux);

            //this will give us the adjugate matrix (j and i are switched)
            if((i+j)%2 == 0){
                adjunct.set(j, i, det3(aux));
            }
            else{
                adjunct.set(j, i, -det3(aux));
            }
            

            //Calculate determinant if it's not calculated
            if(j == 0 && !detCalculated){
                //at this point, adjunct contains the necessary elements to get the determinant
                // (the determinants of a matrix can be obtained with its cofactors,
                // since a cofactor is the determinant of a minor obtained by removing the ith and jth
                // row and column of a matrix with alternating signs)
                //adjunct is transposed, so we need the switch the indices
                //we will use the first column as "pivot"
                det += m[i][j] * adjunct.get(j, i);
            }
        }
    }
    //Saves determinant if it hadn't been previously calculated
    if(!detCalculated){
        determinant = det;
        detCalculated = true;
    }
    
    if(det == 0) throw NoInverseException(*this);
    
    return (1.0/det) * adjunct;
}

//***********************************************************************
// Removes a row and a column from this matrix and stores it in a 3x3 array
// @param iR Row to be removed
// @param jR Column to be removed
// @param mat[3][3] Matrix where the new 3x3 matrix will be stored (resulting of removing row iR and column jR from this matrix)
//***********************************************************************
void Matrix4::removeRowColumn(int iR, int jR, float mat[3][3]){
    int aux_i = 0, aux_j = 0;
    for(int i = 0; i < 4; i++){
        if(i != iR){
            aux_j = 0;
            for(int j = 0; j < 4; j++){
                if(j != jR){
                    mat[aux_i][aux_j] = m[i][j];
                    aux_j++;
                }
            } 
            aux_i++;
        }
    }
}

//***********************************************************************
// @param i Horizontal component
// @param j Vertical component
// @returns The value of the ij component of this matrix
//***********************************************************************
float Matrix4::get(const int i, const int j) const{
    return m[i][j];
}

//***********************************************************************
// Adds two 2x2 matrices
// @param a[2][2] first matrix
// @param b[2][2] second matrix
// @param c[2][2] Will store the result of adding a + b
//***********************************************************************
void Matrix4::add2(const float a[2][2], const float b[2][2], float c[2][2]){
    c[0][0] = a[0][0] + b[0][0];
    c[0][1] = a[0][1] + b[0][1];
    c[1][0] = a[1][0] + b[1][0];
    c[1][1] = a[1][1] + b[1][1];
}

//***********************************************************************
// Substracts two 2x2 matrices
// @param a[2][2] first matrix
// @param b[2][2] second matrix
// @param c[2][2] Will store the result of performing a - b
//***********************************************************************
void Matrix4::substract2(const float a[2][2], const float b[2][2], float c[2][2]){
    c[0][0] = a[0][0] - b[0][0];
    c[0][1] = a[0][1] - b[0][1];
    c[1][0] = a[1][0] - b[1][0];
    c[1][1] = a[1][1] - b[1][1];
}

//***********************************************************************
// Calculates the product of two 4x4 matrices. Uses Divide And Conquer algorithm
// @param operand second matrix
// @returns The product of this matrix * operand
//***********************************************************************
Matrix4 Matrix4::operator* (const Matrix4& operand){
    Matrix4 res;

    //division in submatrices of this
    float a[2][2], b[2][2], c[2][2], d[2][2];
    //division in submatrices of operand
    float e[2][2], f[2][2], g[2][2], h[2][2];

    quarter(0, a);
    quarter(1, b);
    quarter(2, c);
    quarter(3, d);
    operand.quarter(0, e);
    operand.quarter(1, f);
    operand.quarter(2, g);
    operand.quarter(3, h);

    //Array of 2x2 submatrices
    float P[7][2][2];
    float aux1[2][2];
    float aux2[2][2];

    //p0 = a*(f-h)
    substract2(f, h, aux1);
    strassen2(a, aux1, P[0]);

    //p1 = (a+b)*h
    add2(a, b, aux1);
    strassen2(aux1, h, P[1]);

    //p2 = (c+d)*e
    add2(c, d, aux1);
    strassen2(aux1, e, P[2]);

    //p3 = d*(g-e)
    substract2(g, e, aux1);
    strassen2(d, aux1, P[3]);

    //p4 = (a+d)+(e+h)
    add2(a, d, aux1);
    add2(e, h, aux2);
    strassen2(aux1, aux2, P[4]);

    //p5 = (b-d)+(g+h)
    substract2(b, d, aux1);
    add2(g, h, aux2);
    strassen2(aux1, aux2, P[5]);

    //p6 = (a-c)+(e+f)
    substract2(a, c, aux1);
    add2(e, f, aux2);
    strassen2(aux1, aux2, P[6]);

    //Array of results
    float R[4][2][2];

    //R0 = p4 + p3 - p1 + p5
    add2(P[4], P[3], aux1);
    substract2(aux1, P[1], aux2);
    add2(aux2, P[5], R[0]);

    //R1 = p0 + p1
    add2(P[0], P[1], R[1]);

    //R2 = p2 + p3
    add2(P[2], P[3], R[2]);

    //R3 = p4 + p0 - p2 - p6
    add2(P[4], P[0], aux1);
    substract2(aux1, P[2], aux2);
    substract2(aux2, P[6], R[3]);

    

    //Set res components to the correspondant quarter component
    int i0, j0;
    for(int k = 0; k < 4; k++){
        i0 = k / 2;
        j0 = k % 2;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                res.set(i + i0*2, j + j0*2, R[k][i][j]);
            }
        }
    }
    
    return res;

}

//***********************************************************************
// Multiplies two 2x2 matrices using Strassen's algorithm
// @param a[2][2] first matrix
// @param b[2][2] second matrix
// @param c[2][2] Will store the result of performing a * b
//***********************************************************************
void Matrix4::strassen2 (const float a[2][2], const float b[2][2], float c[2][2]){
    float aux[7];

    aux[0] = (a[0][0] +  a[1][1]) * (b[0][0] + b[1][1]);
    aux[1] = (a[1][0] +  a[1][1]) *  b[0][0];
    aux[2] =  a[0][0] * (b[0][1]  -  b[1][1]);
    aux[3] =  a[1][1] * (b[1][0]  -  b[0][0]);
    aux[4] = (a[0][0] +  a[0][1]) *  b[1][1];
    aux[5] = (a[1][0] - a[0][0])  * (b[0][0] + b[0][1]);
    aux[6] = (a[0][1] - a[1][1])  * (b[1][0] + b[1][1]);

    c[0][0] = aux[0] + aux[3] - aux[4] + aux[6];
    c[0][1] = aux[2] + aux[4];
    c[1][0] = aux[1] + aux[3];
    c[1][1] = aux[0] - aux[1] + aux[2] + aux[5];
}

//***********************************************************************
// Returns a 2x2 matrix corresponding to a 2x2 corner of "quadrant" of this 4x4 matrix
// @param part The "quadrant" to be returned
// @param sun[2][2] Will store the indicated quadrant
//***********************************************************************
void Matrix4::quarter(const int part, float sub[2][2]) const{
    int i0 = part / 2;  //Row of submatrices
    int j0 = part % 2;  //Column of submatrices

    //indices between 0 y 1
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            //sub[i][j] will be the i,j component of quarter i0,j0
            sub[i][j] = m[i + i0*2][j + j0*2];
        }
    } 
}
//***********************************************************************
// Sets component i,j of tihs matrix to value n
// @param i Horizontal component
// @param j Vertical component
// @param n New value
//***********************************************************************
void Matrix4::set(const int i, const int j, const float n){
    m[i][j] = n;
}

//***********************************************************************
//Sets column j of matrix m to c
// @param j Column
// @param c Array containing the new value for row j
//***********************************************************************
void Matrix4::setColumn(const int j, const float c[4]){
    for(int i = 0; i < 4; i++){
        m[i][j] = c[i];
    }
}

//***********************************************************************
// Class constructor
// @param mat Matrix that caused the exception
//***********************************************************************
NoInverseException::NoInverseException(const Matrix4& mat){
    this->mat = mat;
}

//***********************************************************************
// Returns the message of the exception
//***********************************************************************
const char* NoInverseException::what() const{
    return "Matrix has no inverse";
}

//***********************************************************************
// Multiplies a matrix by a scalar
// @param scalar The scalar
// @param mat The matrix
// @returns A matrix with the result of performing scalar * mat
// @relatealso Matrix4
//***********************************************************************
Matrix4 operator* (const float scalar, Matrix4 mat){
    Matrix4 result;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            result.set(i, j, scalar * mat.get(i, j));
        }
    }
    return result;
}

//***********************************************************************
// Prints on the specified output stream the matrix, surrounded by "|"
// @param os An output stream
// @param mat Some matrix
// @relatealso Matrix4
// @returns A reference to ostram os (allows for concatenation of operations)
//***********************************************************************
ostream& operator<<(ostream& os, const Matrix4& mat){
    for(int i = 0; i < 4; i++){
        cout << "|" << flush;
        for(int j = 0; j < 4; j++){
            cout << setw(10) << mat.get(i, j) << flush;
        }
        cout << "|" << endl;
    }
    return os;
}