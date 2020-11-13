#ifndef MATRIX4
#define MATRIX4

#include <iostream>
#include <iomanip>

using namespace std;

class Matrix4{
    private:
        float m[4][4];
        float determinant;
        bool detCalculated = false;

        float det3(float mat[3][3]);
        void strassen2 (const float a[2][2], const float b[2][2], float c[2][2]);
        void add2(const float a[2][2], const float b[2][2], float c[2][2]);
        void substract2(const float a[2][2], const float b[2][2], float c[2][2]);

    public:
        float det();
        
        void identity();
        Matrix4 inverse();
        void removeRowColumn(int iR, int jR, float mat[3][3]);
        float get(const int i, const int j) const;
        Matrix4 operator* (const Matrix4& operand);
        void quarter(const int part, float sub[2][2]) const;
        void set(const int i, const int j, const float n);
        void setColumn(const int j, const float c[4]);
};

class NoInverseException{
    private:
        Matrix4 mat;

    public:
        NoInverseException(const Matrix4& mat);
        const char* what() const;
};

Matrix4 operator* (float scalar, Matrix4 mat);
ostream& operator<<(ostream& os, const Matrix4& mat);

#endif