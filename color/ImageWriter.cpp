//File with the necessary functions to write an image on an output file

#include "ImageWriter.hpp"
// Character that identifies a comment in PPM files
#define COMMENT '#'
// Sequence that identifies a comment that indicates the Max values of LDR files to be translated into HDR
#define MAX_COMMENT "#MAX="

// Size of the BMP format header
#define BMP_HEADER_SIZE 14
//Size of the information fields in BMP format
#define BMP_INFO_SIZE 40
//Bits per pixel in the BMP format
#define BMP_BITS_PER_PIXEL 24

//***********************************************************************
//Writes size bytes (binary) of obj into fout
// @tparam T type of the object to be written
// @param fout Output file stream
// @param obj Object to be written
// @param size Size of the object. Defaults to the obj's size.
//***********************************************************************
template <typename T>
void binWrite(ofstream& fout, T obj, int size = -1){
    if(size == -1) size = sizeof(obj);
    fout.write(reinterpret_cast<const char*>(&obj), size);
}

//***********************************************************************
//Writes size bytes (binary) of *obj into fout
// @tparam T type of the object to be written
// @param fout Output file stream
// @param obj Pointer to the object to be written
// @param size Size of the object. Defaults to the obj's size.
//***********************************************************************
template <typename T>
void binWrite(ofstream& fout, T* obj, int size = -1){
    if(size == -1) size = sizeof(*obj);
    fout.write(reinterpret_cast<const char*>(obj), size);
}

//***********************************************************************
// Writes the image img in a file with name "output" in PPM format
// @param img Image to be written
// @param output Name of the output file
//***********************************************************************
void generatePPM(Image& img, string output){

    ofstream fout(output);

    if(fout.is_open()){
        cout << "Generating " << output << endl;
        int width = img.getWidth();
        int height = img.getHeight();
        float max = img.getMax();

        float red;
        float green;
        float blue;

        fout << img.getFormat() << endl << MAX_COMMENT << max << endl << width << " " << height << endl << LDR_RES << endl;
        cout << fixed << setprecision(2);
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                red = img.getTuple(i, j)[0];
                green = img.getTuple(i, j)[1];
                blue = img.getTuple(i, j)[2];

                fout << img.memoryToDisk(red) << " ";
                fout << img.memoryToDisk(green) << " ";
                fout << img.memoryToDisk(blue) << "  ";
            }

            cout << "\r " << flush;
            cout << 100.0 * (j*width) / (width*height) << "%";
            
            fout << endl;
        }
        cout << "\r 100.00%" << endl;
    }
    else{
        cerr << "Couldn't write on " << output << endl;
    }

    fout.close();
}

//***********************************************************************
// Writes the image img in a file with name "output" in Bitmap format
// @param img Image to be written
// @param output Name of the output file
//***********************************************************************
void generateBMP(Image& img, string output){

    ofstream fout(output, ios::binary);

    if(fout.is_open()){
        cout << "Generating " << output << endl;
        int width = img.getWidth();
        int height = img.getHeight();

        //FILE HEADER
        binWrite(fout, "BM", 2);    //Signature
        binWrite(fout, BMP_HEADER_SIZE + BMP_INFO_SIZE + width*height*3 + width*3 % 4 * height);   //FileSize (HEADER + INFO + PIXELS + PADDINGS)
        binWrite(fout, 0); //Reserved
        binWrite(fout, BMP_HEADER_SIZE + BMP_INFO_SIZE);    //DataOffset

        //INFO HEADER
        binWrite(fout, BMP_INFO_SIZE);  //Size (of Info Header)
        binWrite(fout, width);  //Width
        binWrite(fout, height); //Height
        binWrite(fout, (short)1); //Number of planes
        binWrite(fout, (short)BMP_BITS_PER_PIXEL);  //Bits per pixel 
        binWrite(fout, 0); //Compression
        binWrite(fout, 0); //ImageSize (if not compressed can be set to 0)
        binWrite(fout, 0); //XpixelsPerM (can be set to 0 if no preference)
        binWrite(fout, 0); //YpixelsPerM (can be set to 0 if no preference)
        binWrite(fout, 1 << BMP_BITS_PER_PIXEL);  //Color resolution (2^bits_per_pixel)
        binWrite(fout, 0); //Important colors (0 = all)

        //COLOR TABLE NOT NECESSARY FOR IMAGES OF MORE THAN 8 COLORS

        cout << fixed << setprecision(2);
        float red, green, blue;
        byte redByte, greenByte, blueByte;
        //PIXEL DATA (stored bottom to top)
        for(int j = height - 1; j >= 0; j--){
            for(int i = 0; i < width; i++){
                red = img.getTuple(i, j)[0];
                green = img.getTuple(i, j)[1];
                blue = img.getTuple(i, j)[2];
                
                redByte = img.memoryToDisk(red);
                greenByte = img.memoryToDisk(green);
                blueByte = img.memoryToDisk(blue);

                binWrite(fout, blueByte);   //blue
                binWrite(fout, greenByte);  //green
                binWrite(fout, redByte);    //red
            }
            for(int p = 0; p < (width*3) % 4; p++){ //each row of pixels must start in a 4-aligned value...
            //...so we add padding each time the row doesn't end in a multiple of 4
                binWrite(fout, (byte)0);
            }
            
            cout << "\r " << flush;
            cout << 100.0 * ((height-j)*width) / (width*height) << "%";
        }
        cout << "\r 100.00%" << endl;
    }
    else{
        cerr << "Couldn't write on " << output << endl;
    }

    fout.close();
}

//***********************************************************************
// Creates a tone mapper of the indicated type and with the arguments provided.
//    Clamp:            setToneMapper(CLAMP, null)
//    Equalize:         setToneMapper(EQUALIZE, {imageMax})
//    EqualizeClamp:    setToneMapper(EQUALIZECLAMP, {V})
//    Gamma:            setToneMapper(GAMMA, {imageMax, gamma})
//    GammaClamp:       setToneMapper(CLAMPGAMMA, {V, gamma})
//    Reinhard2002:     setToneMapper(REINHARD2002, null)
//    Reinhard2005:     setToneMapper(REINHARD2005, {imageMax})
// @param tmt Tone mapper technique to be used 
// @param args Vector with the arguments required by the tone mapper
//***********************************************************************
ToneMapperPtr createToneMapper(ToneMappingType tmt, float* args){
    ToneMapperPtr tm;
    switch(tmt){
        case CLAMP:
            tm = make_shared<Clamp>();
            break;
        case EQUALIZE:
        case EQUALIZECLAMP:
            tm = make_shared<EqualizeAndClamp>(args[0]);
            break;
        case GAMMA:
        case CLAMPGAMMA:
            tm = make_shared<GammaAndClamp>(args[1], args[0]); 
            break;
        case REINHARD2002:
            tm = make_shared<Reinhard2002>();
            break;
        case REINHARD2005:
            tm = make_shared<Reinhard2005>(args[0]);
            break;
    }
    return tm;
}

//***********************************************************************
// Writes the image to the specified output file
// @param img Image to be written
// @param outputFile Path of the output file
// @param tmt Tone mapping technique to be used
// @param destinationColorRes Color resolution of the output file
// @param bmp When set to true, the output file will be in Bitmap format. Otherwise, HDR format.
// @param toneMapperArgs Specifies arguments required by the tone mapper (see createToneMapper for more info)
//***********************************************************************
void writeImage(Image& img, string outputFile, ToneMappingType tmt, int destinationColorRes, bool bmp, float* toneMapperArgs){

    img.setToneMapper(createToneMapper(tmt, toneMapperArgs));
    img.setDestinationResolution(destinationColorRes);
    cout << "Mapping..." << endl;
    img.applyToneMapper();

    if(bmp){
        generateBMP(img, outputFile);
    }
    else{
        generatePPM(img, outputFile);
    }
    
    cout << "Finished!" << endl;
}

