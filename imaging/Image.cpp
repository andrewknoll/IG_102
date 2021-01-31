#include "Image.hpp"

//***********************************************************************
// Function to obtain data from LDR files comments
// @param s String containing the comments in an LDR file
// @returns Value specified by the #MAX comment.
//          -1 if no value has been specified
//***********************************************************************
float Image::parseComment(string s){
    float retVal = -1;
    int last = s.find_last_of("MAX=");

    if(last != -1){
        istringstream stream(s.substr(last + 1));
        stream >> retVal;
    }
    return retVal;
}

//***********************************************************************
// Translates the value stored in an LDR into an HDR value
// @param i LDR value
// @returns HDR value corresponding to i
//***********************************************************************
float Image::diskToMemory(int i){
    return i * max/colorRes;
}

//***********************************************************************
// Sets the value of the width and the height of the image.
// Resize matrix to the specified size.
// @param width Width of the image.
// @param height Height of the image.
//***********************************************************************
void Image::setWidthHeight(int width, int height){
    this->width = width;
    this->height = height;
    m.resize(width);
    for(int i = 0; i < width; i++){
        m[i].resize(height);
    }
}

//***********************************************************************
// Translates the value stored in an HDR into an LDR value
// @param f HDR value
// @returns ldr value corresponding to f
//***********************************************************************
int Image::memoryToDisk(float f){
    int aux = round(f * destinationCR/maxAfterMapping);
    return aux;
}

//***********************************************************************
// Format getter
// @returns format string
//***********************************************************************
string Image::getFormat(){
    return format;
}

//***********************************************************************
// Width getter
// @returns width of the image
//***********************************************************************
int Image::getWidth(){
    return width;
}

//***********************************************************************
// Height getter
// @returns height of the image
//***********************************************************************
int Image::getHeight(){
    return height;
}

//***********************************************************************
// Max getter
// @returns max
//***********************************************************************
float Image::getMax(){
    return max;
}

//***********************************************************************
// Mean getter
// @returns valuesMean
//***********************************************************************
float Image::getMean(){
    return valuesMean;
}

//***********************************************************************
// Sets the value of maxFound
// @param n New value for maxFound
//***********************************************************************
void Image::setMaxFound(float n){
    maxFound = n;
}

//***********************************************************************
// maxFound getter
// @returns maxFound
//***********************************************************************
float Image::getMaxFound(){
    return maxFound;
}

//***********************************************************************
// Color resolution getter
// @returns colorRes
//***********************************************************************
int Image::getColorRes(){
    return colorRes;
}

//***********************************************************************
// Sets the value of the destination color resolution
// @param cr New value for destinationCR
//***********************************************************************
void Image::setDestinationResolution(int cr){
    destinationCR = cr;
}

//***********************************************************************
// Sets the value of maxAfterMapping
// @param n New value for maxAfterMapping
//***********************************************************************
void Image::setMaxAfterMapping(float n){
    maxAfterMapping = n;
}

//***********************************************************************
// maxAfterMapping getter
// @returns maxAfterMapping
//***********************************************************************
float Image::getMaxAfterMapping(){
    return maxAfterMapping;
}

//***********************************************************************
// Sets the tone mapping technique
// @param tm Smart pointer to the tone mapper to be used
//***********************************************************************
void Image::setToneMapper(shared_ptr<ToneMapper> tm){
    this->tm = tm;
}

//***********************************************************************
// Applies the tone mapper technique to all the image.
// tm must not be null.
//***********************************************************************
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
//***********************************************************************
// Returns a specified RGB tuple from the matrix
// @param i Horizontal component of the RGB tuple
// @param j Vertical component of the RGB tuple
// @returns m[i][j]
//***********************************************************************
RGB Image::getTuple(int i, int j){
    return m[i][j];
}

//***********************************************************************
// Sets the value of a specified RGB tuple from the matrix
// @param color New RGB value to be set to the tuple
// @param i Horizontal component of the RGB tuple
// @param j Vertical component of the RGB tuple
//***********************************************************************
void Image::setTuple(RGB color, int i, int j){
    m[i][j] = color;
    if(maxFound < color.max()){
        maxFound = color.max();
    }
}

//***********************************************************************
// Reads an LDR image file (in PPM) and saves it in the matrix on HDR format.
// File must have a comment specifying the maximum luminance value for the translation to occur.
// @param filename Name of the file to be read
//***********************************************************************
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

//***********************************************************************
// Prints through the standard output the tuples of the image for debug purposes.
//***********************************************************************
void Image::toString(){
    cout << format << endl << max << endl << width << endl << height << endl << colorRes << endl;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            m[i][j].toString();
            cout << "    " << endl;
        }
    }
}