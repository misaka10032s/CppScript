#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 

#include <windows.h>
#include <ctime>

#include <opencv2/opencv.hpp>

#include <cstdint>
#include <cstring>
#include <vector>

#include <string>
#include <sstream>

#include "imgproc.cpp"
// #include "KBDscript.cpp"
#include "sysfunc.cpp"

using namespace cv;
using namespace std;

bool checkcost(vector<uint8_t>& Pixels, int width, int height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, e = 0;
    int posx = 64, posy = 0, wid = 4, hei = 15, val = 55845; // $100000 => 47430, $15000 => 55845
    vector<uint8_t> test;
    test.resize(width * height * bit);
    for (int j = 0; j < height; j++) {
        for(int i=0;i<width;i++){
            for(int k=0;k<3;k++){
                test[i*bit + j*width*bit + k] = Pixels[i*bit + j*width*bit + k];
            }
        }
    }

    for (int j = 0; j <= hei; j++) {
        for(int i=0;i<=wid;i++){
            for(int k=0;k<3;k++){
                e += Pixels[(posx+i)*bit + (posy+j)*width*bit + k];
                test[(posx+i) * bit + (posy+j) * width * bit + k] = 0;
                // cout << e << " " << (int)Pixels[(posx+i) * bit + (posy+j) * width * bit + k] << "\n";
            }
        }
    }
    // cout << e << "\n";
    // imshow("Display w", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &test[0]));
    // waitKey(0);
    return e < val ? 1 : 0;
}

int main(){
    vector<uint8_t> cost;
    int BitsPerPixel, width = 160, height = 15, posx = 605, posy = 408;
    int washX = 660, washY = 530;
    int putX = 680, putY = 480; // @@
    int bagX = 27, bagY = 90, dxy = 43;
    bool enable = 0;

    set_key_value();

    while(1){
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            for (int row = 0; row < 6;row++){
                for (int col = 0; col < 4;col++){
                    click(bagX + dxy * col, bagY + dxy * row, "ALL");
                    syscontrol(200, &nowtime, &enable, &nowtick);
                    click(putX, putY, "ALL"); 
                    syscontrol(400, &nowtime, &enable, &nowtick);

                    while(1){
                        ScreenShot(cost, BitsPerPixel, width, height, posx, posy);
                        // imshow("Display w", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &cost[0]));
                        // waitKey(0);
                        // checkcost(cost, width, height, BitsPerPixel);
                        if (checkcost(cost, width, height, BitsPerPixel)){
                            cout<<"rare\n";
                            break;
                        }
                        else{
                            click(washX, washY, "ALL");
                            syscontrol(400, &nowtime, &enable, &nowtick);
                            keybd("ENTER", "ALL");
                            syscontrol(2000, &nowtime, &enable, &nowtick);
                        }
                    }
                }
            }
            enable = 0;
            cout << "done\n";
        }
    }
    return 0;
}