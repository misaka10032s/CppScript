#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <map>
#include <windows.h>
#include <ctime>

#include <opencv2/opencv.hpp>

#include <cstdint>
#include <cstring>
#include <vector>

#include <string>
#include <sstream>

#include "imgproc.cpp"
#include "KBDscript.cpp"
#include "sysfunc.cpp"

bool checkTTT(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    std::vector<uint8_t> checkTTTPixels, spePixels;
    // 195 0.205263
    // 495 0.309375
    // 238 0.198333
    // 421 0.2105
    // 266 0.221667

    struct point{
        int x;
        int y;
    };
    std::vector<point> record;
    int range[5][4] = { // left right top bottom
        {27,65,0,25},
        {10,90,38,58},
        {20,80,78,98},
        {0,100,118,138},
        {20,80,157,177},
    }, dots[5] = {0,0,0,0,0}, outside, dx = 15, dy = 15, wid = 102, hei = 177, idx = 0;
    float rate[5], outsiderate;
    bool isin, isfit;
    checkTTTPixels.resize(wid * hei * bit);
    // struct timeval timestamp, aftertimestamp;
    // gettimeofday(&timestamp, NULL);

    for(int y=0;y<Height-hei;y+=dy){
        for(int x=0;x<Width-wid;x+=dx){
            for(int p=0;p<5;p++) dots[p] = 0;
            isfit = 1;
            outside = 0;
            
            for(int j=0;j<hei;j++){
                for(int i=0;i<wid;i++){
                    isin = 0;
                    for(int p=0;p<5;p++){
                        if(i > range[p][0] && i < range[p][1] && j > range[p][2] && j < range[p][3]){
                            if(Pixels[(x+i)*bit + (y+j)*Width*bit] == 255) dots[p]++;
                            isin = 1;
                            break;
                        }
                    }
                    if(!isin){
                        if(Pixels[(x+i)*bit + (y+j)*Width*bit] == 255) outside++;
                    }
                }
            }

            for(int p=0;p<5;p++) {
                rate[p] = dots[p] / (float)((range[p][1]-range[p][0])*(range[p][3]-range[p][2]));
            }
            outsiderate = outside / 11104.0;

            for(int p=0;p<5;p++){
                isfit &= (rate[p]>0.2 /*&& rate[p]<0.23*/);
            }
            isfit &= outsiderate<0.08;
            
            if(isfit){
                record.resize(idx+1);
                record[idx].x = x;
                record[idx].y = y;
                idx++;
            }
            // cutImg(Pixels, checkTTTPixels, Width, Height, BitsPerPixel, wid, hei, x, y);
            // imshow("Display matching", Mat(hei, wid, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &checkTTTPixels[0]));
            // for(int p=0;p<5;p++){
            //     cout<<rate[p]<<" ";
            // }
            // cout<<outsiderate<<"\n";
            // waitKey(0);
        }
    }
    // gettimeofday(&aftertimestamp, NULL);
    // long int ms = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
    // long int msa = aftertimestamp.tv_sec * 1000 + aftertimestamp.tv_usec / 1000;
    // cout<<"\ncost time: "<<msa-ms<<" ms\n";

    // for(int i=0;i<idx;i++){
    //     cout<<record[i].x<<" "<<record[i].y<<"\n";
    //     cutImg(Pixels, checkTTTPixels, Width, Height, BitsPerPixel, wid, hei, record[i].x, record[i].y);
    //     special(checkTTTPixels, spePixels, wid, hei, BitsPerPixel);
    //     imshow("Display spe", Mat(hei, wid, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &spePixels[0]));
    //     imshow("Display matching", Mat(hei, wid, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &checkTTTPixels[0]));
    //     waitKey(0);
    // }
    if(idx)return 1;
    return 0;
}

void sixteen(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& sixteenPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    sixteenPixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            for(int k=0;k<3;k++){
                sixteenPixels[i*bit + j*Width*bit + k] = Pixels[i*bit + j*Width*bit + k] - Pixels[i*bit + j*Width*bit + k]%64;
            }
        }
    }
}

void filter(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& filterPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    std::vector<uint8_t> blurPixels;
    filterPixels.resize(Width * Height * bit);
    // GaussBlur(Pixels, blurPixels, Width, Height, BitsPerPixel, 0.5);
    // struct timeval timestamp, aftertimestamp;
    // gettimeofday(&timestamp, NULL);

    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            if(Pixels[i*bit + j*Width*bit + 0] < 100 && abs(Pixels[i*bit + j*Width*bit + 1] - Pixels[i*bit + j*Width*bit + 2] + 50) < 70 && Pixels[i*bit + j*Width*bit + 1] + Pixels[i*bit + j*Width*bit + 2] > 270){
                for(int k=0;k<3;k++){
                    filterPixels[i*bit + j*Width*bit + k] = 255;
                }
            }
            else{
                for(int k=0;k<3;k++){
                    filterPixels[i*bit + j*Width*bit + k] = 0;
                }
            }
        }
    }
    // gettimeofday(&aftertimestamp, NULL);
    // long int ms = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
    // long int msa = aftertimestamp.tv_sec * 1000 + aftertimestamp.tv_usec / 1000;
    // cout<<"\ncost time: "<<msa-ms<<" ms\n";
}

int main(){
    bool enable = 0;
    std::vector<uint8_t> map ,tmpmap;
    int BitsPerPixel, width = 1366, height = 768, posx = 0, posy = 0, dtime = 0;
    while(1) {
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            ScreenShot(map, BitsPerPixel, width, height, posx, posy);
            sixteen(map, tmpmap, width, height, BitsPerPixel);
            filter(tmpmap, map, width, height, BitsPerPixel);
            if(checkTTT(map, width, height, BitsPerPixel)){
                cout << "\a";
                cout << dtime++ << "\n";
            }
            syscontrol(1000, &nowtime, &enable, &nowtick);
        }
    }
    return 0;
}