#include "ImageWriter.hpp"

#define COMMENT '#'
#define MAX_COMMENT "#MAX="

#define BMP_HEADER_SIZE 14
#define BMP_INFO_SIZE 40
//(256 possible intensities for each RGB component)
#define BMP_BITS_PER_PIXEL 24

//Writes size bytes of obj into fout
template <typename T>
void binWrite(ofstream& fout, T obj, int size = -1){
    if(size == -1) size = sizeof(obj);
    fout.write(reinterpret_cast<const char*>(&obj), size);
}

//Pointer Overload
template <typename T>
void binWrite(ofstream& fout, T* obj, int size = -1){
    if(size == -1) size = sizeof(*obj);
    fout.write(reinterpret_cast<const char*>(obj), size);
}


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
                red = img.getTuple(i, j).get(0);
                green = img.getTuple(i, j).get(1);
                blue = img.getTuple(i, j).get(2);

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
                red = img.getTuple(i, j).get(0);
                green = img.getTuple(i, j).get(1);
                blue = img.getTuple(i, j).get(2);
                
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

//Clamp:            setToneMapper(CLAMP, null)
//Equalize:         setToneMapper(EQUALIZE, {imageMax})
//EqualizeClamp:    setToneMapper(EQUALIZECLAMP, {V})
//Gamma:            setToneMapper(GAMMA, {imageMax, gamma})
//GammaClamp:       setToneMapper(CLAMPGAMMA, {V, gamma})
ToneMapperPtr createToneMapper(ToneMappingType tmt, float *args){
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
    }
    return tm;
}


void writeImage(Image& img, string outputFile, ToneMappingType tmt, int destinationColorRes, bool bmp, float *toneMapperArgs){

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

