//INCOMPLETO

//test creación planetas
float v[9];

    for(int i = 0; i < 9; i++){
        cin >> v[i];
        cout << "He cogido v[" << i << "]" << endl;
    }
    Planet p;
    p.setCoords(v);

    cout << "Yay" << endl;

    return 0;

//test cambio de base
    float v[15];

    for(int i = 0; i < 15; i++){
        if(i == 0) cout << "Original point / direction:" << endl;
        else if(i == 3){
            
            cout << "Axis u:" << endl;
        }
        else if(i == 6){
            
            cout << "Axis v:" << endl;
        }
        else if(i == 9){
            
            cout << "Axis w:" << endl;
        }
        else if(i == 12){
            
            cout << "Origin point:" << endl;
        }
        cin >> v[i];
        cout << "He cogido v[" << i << "]" << endl;
    }
    Point original(v[0], v[1], v[2]);
    Direction ax_u(v[3], v[4], v[5]);
    Direction ax_v(v[6], v[7], v[8]);
    Direction ax_w(v[9], v[10], v[11]);
    Point o(v[12], v[13], v[14]);

    Coordinates result = baseChange(original, o, ax_u, ax_v, ax_w);


    cout << result << endl;

//Inversa:
Matrix4 mat;
    float mat3[3][3];
    Matrix4 inv;

    float a[2][2], b[2][2], c[2][2];

    srand(0);

    mat.set(0, 0, 1);
    mat.set(0, 1, 1);
    mat.set(0, 2, 1);
    mat.set(0, 3, 0);

    mat.set(1, 0, 0);
    mat.set(1, 1, 3);
    mat.set(1, 2, 1);
    mat.set(1, 3, 2);

    mat.set(2, 0, 2);
    mat.set(2, 1, 3);
    mat.set(2, 2, 1);
    mat.set(2, 3, 0);

    mat.set(3, 0, 1);
    mat.set(3, 1, 0);
    mat.set(3, 2, 2);
    mat.set(3, 3, 1);

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            a[i][j] = i*2 + j;
            b[i][j] = a[i][j];
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            cout << c[i][j] << endl;
        }
    }


    cout << "Original Matrix: " << endl;
    cout << mat << endl;

    try{
        inv = mat.inverse();
    }
    catch(NoInverseException nie){
        cout << nie.what() << endl;
    }
    

    cout << "Determinant: " << endl;
    cout << mat.det() << endl;

    cout << "Inverse Matrix: " << endl;
    cout << inv << endl;

    cout << "Multiplication: " << endl;
    cout << mat * inv << endl;
    