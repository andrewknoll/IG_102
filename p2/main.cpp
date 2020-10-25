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

using namespace std;

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
template <typename T>
void binWrite(ofstream& fout, T obj, const int size = sizeof(T)){
    fout.write(reinterpret_cast<char*>(obj), size);
}


//HACER EL TONE MAPPER EN MEMORIA (APLICAR A LA MATRIZ DIRECTAMENTE Y LUEGO ESCRIBIR)
void generateOutput(Image& img, string output){

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
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                red = img.getTuple(i, j).get(0);
                green = img.getTuple(i, j).get(1);
                blue = img.getTuple(i, j).get(2);

                fout << img.memoryToDisk(red) << " ";
                fout << img.memoryToDisk(green) << " ";
                fout << img.memoryToDisk(blue) << "  ";
            }

            cout << "\r " << flush;
            cout << 100.0 * (i*height) / (width*height) << "%";
            
            fout << endl;
        }
        cout << "\r 100.00%" << endl;
    }
    else{
        cerr << "Couldn't write on " << output << endl;
    }

    fout.close();
}

/*void generateBMP(Image& img, string output, float max, float V, float gamma){

    ofstream fout(output, ios::binary);

    if(fout.is_open()){
        cout << "Generating " << output << endl;
        int width = img.getWidth();
        int height = img.getHeight();
        float max = img.getMax();

        binWrite(fout, "BM", 2);
        binWrite(fout, BMP_HEADER_SIZE + BMP_INFO_SIZE + width*height*4);
        binWrite(fout, (int)0);
        binWrite(fout, BMP_HEADER_SIZE + BMP_INFO_SIZE);

        binWrite(fout, BMP_INFO_SIZE);
        binWrite(fout, width);
        binWrite(fout, (int)1);
        binWrite(fout, ceil(log2(img.getColorRes())));
        binWrite(fout, (int)0);
        binWrite(fout, (int)0);
        binWrite(fout, (int)0);
        binWrite(fout, (int)0);
        binWrite(fout, img.getColorRes());
        binWrite(fout, (int)0);

        for(int i = 0; i < img.getColorRes(); i++){
            binWrite(fout, );
            binWrite(fout, (int)0);
            binWrite(fout, (int)0);
            binWrite(fout, (int)0);
        }

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){

                float red = img.getTuple(i, j).get(0);
                float green = img.getTuple(i, j).get(1);
                float blue = img.getTuple(i, j).get(2);

                binWrite(fout, img.memoryToDisk(map(max, V, gamma, red)));
                binWrite(fout, (int)0);
                binWrite(fout, (int)0);
                binWrite(fout, (int)0);

                fout << img.memoryToDisk(map(max, V, gamma, red)) << " " << flush;
                fout << img.memoryToDisk(map(max, V, gamma, green)) << " " << flush;
                fout << img.memoryToDisk(map(max, V, gamma, blue)) << "     " << flush;
            }

            cout << "\r " << flush;
            cout << 100.0 * (i*height) / (width*height) << "%";
            
            fout << endl;
        }
        cout << '\r' << endl;
    }
    else{
        cerr << "Couldn't write on " << output << endl;
    }

    fout.close();
}
*/
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

        /*if(strcmp(argv[argc-1], "-bmp")==0){
            generateBMP();
        }
        else{*/
            generateOutput(img, output);
        //}

        
        cout << "Finished!" << endl;
    }
    else{
        cerr << "Input or output arguments not specified" << endl;
        exit(1);
    }
    return 0;
}

