#include "Image.hpp"

void Image::setWidthHeight(int width, int height){
    this->width = width;
    this->height = height;
    m.resize(width);
    for(int i = 0; i < width; i++){
        m[i].resize(height);
    }
}
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

float Image::getMean(){
    return valuesMean;
}

void Image::setMaxFound(float n){
    maxFound = n;
}

float Image::getMaxFound(){
    return maxFound;
}

void Image::setMaxAfterMapping(float n){
    maxAfterMapping = n;
}

float Image::getMaxAfterMapping(){
    return maxAfterMapping;
}

int Image::getColorRes(){
    return colorRes;
}

void Image::setDestinationResolution(int cr){
    destinationCR = cr;
}

RGB Image::getTuple(int i, int j){
    return m[i][j];
}

void Image::setTuple(RGB color, int i, int j){
    m[i][j] = color;
    if(maxFound < color.max()){
        maxFound = color.max();
        cout << color.max() << endl;
    }
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
    int aux = round(f * destinationCR/maxAfterMapping);
    return aux;
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
                        if(rgbReal[k] > maxFound){
                            maxFound = rgbReal[k];
                        }
                        valuesMean += rgbReal[k] / (width*height*3);
                    }
                    RGB pixel(rgbReal[0], rgbReal[1], rgbReal[2]);
                    m[i][j] = pixel;
                }
            }
        }
        f.close();
    }
    else{
        cerr << "Error" << endl;
    }
}

void Image::setToneMapper(shared_ptr<ToneMapper> tm){
    this->tm = tm;
}

void Image::applyToneMapper(){
    if(tm != nullptr){
        maxAfterMapping = 0.0;
        RGB tuple;
        float color;
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                tuple = m[i][j];
                for(int k = 0; k < 3; k++){

                    color = tm->mapValue(tuple[k]);
                    tuple[k] = tm->mapValue(color);
                    
                    if(color > maxAfterMapping){
                        maxAfterMapping = color;
                    }
                }
                m[i][j] = tuple;
            }
        }
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