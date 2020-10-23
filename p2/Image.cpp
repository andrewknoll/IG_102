#include "Image.hpp"


string Image::getFormat(){
    return format;
}

int Image::getWidth(){
    return width;
}

int Image::getHeight(){
    return height;
}

float Image::getMax(){
    return max;
}

int Image::getColorRes(){
    return colorRes;
}

RGB Image::getTuple(int i, int j){
    return m[i][j];
}

float Image::parseComment(string s){
    float retVal = -1;
    int last = s.find_last_of("MAX=");

    if(last != -1){
        istringstream stream(s.substr(last + 1));
        stream >> retVal;
    }
    return retVal;
}

float Image::diskToMemory(int i){
    return i * max/colorRes;
}

int Image::memoryToDisk(float f){
    return round(f * colorRes/max);
}

void Image::read(string filename){
    ifstream f(filename);

    string buffer;
    float aux;

    bool whRead = false;
    bool colorRead = false;

    if(f.is_open()){
        cout << "Reading image..." << endl;
        getline(f, format);
        while(!f.eof() && (!whRead || !colorRead)){
            getline(f, buffer);

            if(buffer[0] == '#'){
                aux = parseComment(buffer);
                if(aux != -1){
                    max = aux;
                }
            }
            else{
                istringstream stream(buffer);
                if(!whRead){
                    stream >> width >> height;
                    m.resize(width);
                    for(int i = 0; i < width; i++){
                        m[i].resize(height);
                    }
                    whRead = true;


                }
                else{
                    stream >> colorRes;
                    colorRead = true;
                }
            }
        }
        //no more comments can be found in the document (starts to read RGB tuples)
        if(!f.eof()){
            for(int i = 0; i < width && !f.eof(); i++){
                for(int j = 0; j < height && !f.eof(); j++){
                    int rgb[3];
                    float rgbReal[3];
                    for(int k = 0; k < 3 && !f.eof(); k++){
                        f >> rgb[k];
                        rgbReal[k] = diskToMemory(rgb[k]);
                    }
                    RGB pixel(rgbReal[0], rgbReal[1], rgbReal[2]);
                    m[i][j] = pixel;
                }
            }
        }
        f.close();
    }
    else{
        cerr << "Auxilio" << endl;
    }
}

void Image::toString(){
    cout << format << endl << max << endl << width << endl << height << endl << colorRes << endl;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            m[i][j].toString();
            cout << "    " << endl;
        }
    }
}