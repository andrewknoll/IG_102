#include "Scene.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Plane.hpp"
#include "vfield/Direction.hpp"
#include "vfield/Point.hpp"
#include "color/RGB.hpp"
#include <math.h>
#include "rayTracer.hpp"
#include <memory>


#define BMP_HEADER_SIZE 14
#define BMP_INFO_SIZE 40
//(256 possible intensities for each RGB component)
#define BMP_BITS_PER_PIXEL 24

typedef unsigned char byte;

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
        binWrite(fout, 1 << BMP_BITS_PER_PIXEL);  //color resolution (2^bits_per_pixel)
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



int main(){
    Direction a(0, 0, 25);
    Point p(0, 0, 70);
    Point point2(17, 0, 60);
    Point point3(-17, 0, 80);
    RGB rgb(255, 0 , 0);
    Point o(0, 20, -6);

    Direction n(0, 0, -1);
    RGB rgb4(0, 50, 50);

    RGB rgb2(0, 255, 0);
    RGB rgb3(0, 0, 255);

    Scene escena(400, 400);
    escena.buildCameraFromVFOV(M_PI/2, o);

    shared_ptr<Sphere> p1 = make_shared<Sphere>(a, p, rgb);
    shared_ptr<Plane> p2 = make_shared<Plane>(n, 90, rgb4);
    shared_ptr<Sphere> p3 = make_shared<Sphere>(a, point2, rgb2);
    shared_ptr<Sphere> p4 = make_shared<Sphere>(a, point3, rgb3);

    escena.addShape(p1);
    escena.addShape(p2);
    escena.addShape(p3);
    escena.addShape(p4);

    Image img;
    img.setWidthHeight(400, 400);
    img.test(255, 255);

    rayTrace(img, escena, 32);

    generateBMP(img, "output.bmp");

    
}