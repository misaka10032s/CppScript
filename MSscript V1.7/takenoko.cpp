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

#include "library/imgproc.cpp"
#include "library/sysfunc.cpp"

using namespace cv;
using namespace std;

int takenoko(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, int ref[15][30][3]){
    int bit = BitsPerPixel > 24 ? 4 : 3, diff = 0;
    vector<uint8_t> koPixels;
    koPixels.resize(Width * Height * bit);

    for(int y=0;y<Height;y++){
        for(int x=0;x<Width;x++){
            for(int k=0;k<3;k++){
                koPixels[x*bit + y*Width*bit + k] = 15 * (Pixels[x*bit + y*Width*bit + k]/15);
            }

            if(koPixels[x*bit + y*Width*bit + 2] >= 165 && abs(koPixels[x*bit + y*Width*bit + 0] - koPixels[x*bit + y*Width*bit + 1]) <= 45 && abs(koPixels[x*bit + y*Width*bit + 2] - koPixels[x*bit + y*Width*bit + 1]) >= 60){
                koPixels[x*bit + y*Width*bit + 0] = 0;
                koPixels[x*bit + y*Width*bit + 1] = 0;
                koPixels[x*bit + y*Width*bit + 2] = 255;
            }
            else{
                for(int k=0;k<3;k++) koPixels[x*bit + y*Width*bit + k] = 0;
            }

            for(int k=0;k<3;k++){
                if(koPixels[x*bit + y*Width*bit + k] != ref[y][x][k]) diff++;
            }
        }
    }
    return diff;
}

int main(){
    vector<uint8_t> focus;
    int BitsPerPixel, width = 30, height = 15, posx = 527, posy = 675, dxmax = 12, nowE = 0;
    int bulbX = 35, bulbY = 355;
    int takenokoX = 460, takenokoY = 400;
    int missionX = 625, missionY = 625;
    int finT = 1, exeTime;
    bool enable = 0, match;
    int checkVal[15][30][3] = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};

    set_key_value();

    cout << "input exeTime:\n";
    cin >> exeTime;
    exeTime = MIN(MAX(exeTime, 0), 5);
    while (1) {
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            cout << finT << "\n";
            if (LOWORD(GetKeyState(VK_NUMLOCK))) {
                keybd("NUMLOCK", "ALL");
                syscontrol(50, &nowtime, &enable, &nowtick);
            }
            syscontrol(200, &nowtime, &enable, &nowtick);
            click(bulbX, bulbY, "ALL");
            syscontrol(200, &nowtime, &enable, &nowtick);
            click(bulbX, bulbY, "ALL");
            syscontrol(200, &nowtime, &enable, &nowtick);
            click(bulbX, bulbY, "ALL");
            syscontrol(200, &nowtime, &enable, &nowtick);

            for (int i = 0; i < 15;i++){
                keybd("DOWN", "ALL");
                syscontrol(50, &nowtime, &enable, &nowtick);
            }
            keybd("ENTER", "ALL");
            syscontrol(600, &nowtime, &enable, &nowtick);
            click(takenokoX, takenokoY, "ALL");
            syscontrol(200, &nowtime, &enable, &nowtick);

            while(1){
                match = 0;
                click(missionX, missionY, "ALL");
                syscontrol(500, &nowtime, &enable, &nowtick);
                keybd("Y", "ALL");
                syscontrol(500, &nowtime, &enable, &nowtick);
                keybd("Y", "ALL");
                syscontrol(500, &nowtime, &enable, &nowtick);

                for (int i = 0; i < dxmax;i++){
                    ScreenShot(focus, BitsPerPixel, width, height, posx + i*16, posy);
                    // nowE = GetEigenvalue(focus, width, height, BitsPerPixel);
                    nowE = takenoko(focus, width, height, BitsPerPixel, checkVal);
                    // cout << nowE << " ";
                    // imshow("Display focus", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &focus[0]));
                    // waitKey(0);
                    if(nowE < 30){ // @@@@
                        match = 1;
                        break;
                    }
                }
                // cout << "\n" << match << "\n";
                if (!match)
                {
                    keybd("ESC", "ALL");
                    syscontrol(500, &nowtime, &enable, &nowtick);
                }
                else{  //**********************************//
                    click(615, 715, "ALL");
                    syscontrol(500, &nowtime, &enable, &nowtick);
                    click(1025, 785, "ALL");
                    syscontrol(500, &nowtime, &enable, &nowtick);
                    click(815, 785, "ALL");
                    syscontrol(500, &nowtime, &enable, &nowtick);
                    keybd("ESC", "ALL");
                    syscontrol(500, &nowtime, &enable, &nowtick);
                    break;
                }
            }
            if (!LOWORD(GetKeyState(VK_NUMLOCK))) {
                keybd("NUMLOCK", "ALL");
                syscontrol(50, &nowtime, &enable, &nowtick);
            }
            syscontrol(31 * 60 * 1000, &nowtime, &enable, &nowtick);

            click(bulbX, bulbY, "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);

            keybd("ENTER", "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);

            click(735, 778, "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);

            finT++;
            if(finT > exeTime){
                enable = 0;
                cout << "finish "<< exeTime <<" times\n";
                // keybd("F10", "ALL");
                break;
            }
        }
    }
    return 0;
}