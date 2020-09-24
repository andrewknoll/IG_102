
struct Matrix4{
    float m[4][4];
    

    Matrix4 operator* (Matrix4& operand){
        Matrix4 res;

        //division in submatrices of this
        float a[2][2], b[2][2], c[2][2], d[2][2];
        //division in submatrices of operand
        float e[2][2], f[2][2], g[2][2], h[2][2];

        submatrix(0, a);
        submatrix(1, b);
        submatrix(2, c);
        submatrix(3, d);
        operand.submatrix(0, e);
        operand.submatrix(1, f);
        operand.submatrix(2, g);
        operand.submatrix(3, h);

        //Array of 2x2 submatrices
        float result[4][2][2];

        strassen2(a, e, result[0]);
        strassen2(b, f, result[1]);
        strassen2(c, g, result[2]);
        strassen2(d, h, result[3]);

        //Set res components to the correspondant submatrix component
        int i0, j0;
        for(int k = 0; k < 4; k++){
            i0 = k / 2;
            j0 = k % 2;
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 2; j++){
                    res.set(i + i0*2, j + j0*2, result[k][i][j]);
                }
            }
        }
        

    }

    //Multiplication of two 2x2 matrices using Strassen's algorithm
    //Stores the result in "c"
    void strassen2 (float a[2][2], float b[2][2], float c[2][2]){
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

    //Obtains a 2x2 matrix in "sub"
    void submatrix(int part, float sub[2][2]){
        int i0 = part / 2;  //Row of submatrices
        int j0 = part % 2;  //Column of submatrices

        //indices between 0 y 1
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                //sub[i][j] will be the i,j component of submatrix i0,j0
                sub[i][j] = m[i + i0*2][j + j0*2];
            }
        } 
    }

    //Sets component i,j of matrix m to value n
    void set(int i, int j, float n){
        m[i][j] = n;
    }

};