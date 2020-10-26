#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <string.h>

#include "Image.hpp"
#include "ToneMapper.hpp"

#ifdef CLAMP
    #include "Clamp.hpp"
#endif
#ifdef EQUALIZE
    #include "Equalize.hpp"
#endif
#ifdef EQUALIZECLAMP
    #include "EqualizeAndClamp.hpp"
#endif
#ifdef GAMMA
    #include "Gamma.hpp"
#endif
#ifdef CLAMPGAMMA
    #include "GammaAndClamp.hpp"
#endif


#define COMMENT '#'
#define MAX_COMMENT "#MAX="

#define LDR_RES 255

#define BMP_HEADER_SIZE 14
#define BMP_INFO_SIZE 40
//(256 possible intensities for each RGB component)
#define BMP_BITS_PER_PIXEL 24

using namespace std;

typedef unsigned char byte;

//tuneMapping
//returns max value
/*float map(Image& img, float max, float V, float g, float val){
    float color[3], maxImage = 0.0;
    for(int i = 0; i < img.getWidth(); i++){
            for(int j = 0; j < img.getHeight(); j++){
                for(int k = 0; k < 3; k++){
                    color[k] = img.getTuple(i, j).get(k);
                #ifdef CLAMP
                    color[k] = clamp(color[k]);
                #endif

                #ifdef EQUALIZE
                    color[k] = equalize(max, color[k]);
                #endif

                #ifdef EQUALIZECLAMP
                    color[k] = equalizeAndClamp(max, V, color[k]);
                #endif

                #ifdef GAMMA
                    color[k] = gamma(max, g, color[k]);
                #endif

                #ifdef CLAMPGAMMA
                    color[k] = clampAndGamma(max, V, g, color[k]);
                #endif

                if(color[k] > maxImage){
                    maxImage = color[k];
                }
            }
        }
    }
    return maxImage;
}
*/

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

        float maxImage = img.getMaxAfterMapping();

        float red;
        float green;
        float blue;

        fout << img.getFormat() << endl << MAX_COMMENT << max << endl << width << " " << height << endl << LDR_RES << endl;
        cout << fixed << setprecision(2);
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                red = img.getTuple(i, j).get(0);
                green = img.getTuple(i, j).get(1);
                blue = img.getTuple(i, j).get(2);

                fout << img.memoryToDisk(red) << " ";
                fout << img.memoryToDisk(green) << " ";
                fout << img.memoryToDisk(blue) << "  ";
            }

            cout << "\r " << flush;
            cout << 100.0 * (i*width) / (width*height) << "%";
            
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
        float max = img.getMax();
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
        for(int i = height - 1; i >= 0; i--){
            for(int j = 0; j < width; j++){
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
            cout << 100.0 * ((height-i)*width) / (width*height) << "%";
        }
        cout << "\r 100.00%" << endl;
    }
    else{
        cerr << "Couldn't write on " << output << endl;
    }

    fout.close();
}


int main(int argc, char** argv){

    string input, output;

    if(argc >= 3){
        input = argv[1];
        output = argv[2];

        float max = -1, gamma = -1, V = -1;

        shared_ptr<ToneMapper> tm;
        
        #ifdef CLAMP
            tm = make_shared<Clamp>();
        #endif

        #ifdef EQUALIZE
            tm = make_shared<Equalize>();
        #endif

        #ifdef EQUALIZECLAMP
            if(argc >= 4){
                V = atof(argv[3]);
                tm = make_shared<EqualizeAndClamp>(V);
            }
            else{
                cerr << "V argument not specified" << endl;
                exit(1);
            }
        #endif

        #ifdef GAMMA
            if(argc >= 4){
                gamma = atof(argv[3]);
                tm = make_shared<Gamma>(gamma);
            }
            else{
                cerr << "GAMMA argument not specified" << endl;
                exit(2);
            }
        #endif

        #ifdef CLAMPGAMMA
            if(argc >= 5){
                V = atof(argv[3]);
                gamma = atof(argv[4]);
                tm = make_shared<GammaAndClamp>(gamma, V);
                
            }
            else{
                cerr << "Insuficient arguments (need V and gamma)" << endl;
                exit(3);
            }
        #endif

        Image img;
        img.read(input);

        tm->apply(img);

        if(strcmp(argv[argc-1], "-bmp")==0){
            generateBMP(img, output);
        }
        else{
            generatePPM(img, output);
        }

        
        cout << "Finished!" << endl;
    }
    else{
        cerr << "Input or output arguments not specified" << endl;
        exit(1);
    }
    return 0;
}

