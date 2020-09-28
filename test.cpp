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