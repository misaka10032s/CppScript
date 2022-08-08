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

int statistics[2][12] = {{0}, {0}}, totalCube = 0;

int checkfit(vector<uint8_t>& Pixels, int width, int height, int BitsPerPixel){
    /*          6%      3%      3%
        str     372262          369885
        dex     374908          373424
        int     373257          371039
        int             372810
        luk     372582          370580
        luk             370172
        all     433772        
        ap      492323  490098
        ad                   

                6%      3%      3%      6%      6%
        str     373558  371603  371450  373507
        dex     376533  374816  374731  376720
        int     374714  372810  372776
        luk     374085  372198  372181  374102
        all     436968
        ad      500888  499188  498916
        ap      497199  495431  495244  497335
        總adp   462077  459952  460054
                3%      3%      3%



        600 427 160 14
        600 441 160 14
        600 455 160 14
    */
    int table[12][3] = {
        {373558, 371603, 371450}, // str
        {376533, 374816, 374731}, // dex
        {374714, 372810, 372776}, // int
        {374085, 372198, 372181}, // luk
        {436968, 436968, 436968}, // all
        {421804, 419713, 420053}, // hp
        {500888, 499188, 498916}, // ad
        {497199, 495431, 495244}, // ap
        {462077, 459952, 460054}, // adp
        {588336, 588183, 588302}, // -def
        {0, 492184, 491997}, // static ad
        {0, 488427, 488325}, // static ap
    };
    int attr[12] = {0}, gate[12] = {9, 9, 9, 9, 6, 12, 9, 9, 12, 100, 100, 100};

    int bit = BitsPerPixel > 24 ? 4 : 3, e = 0;
    int rowHeight = 14, minV, row, col, anotherOK;
    vector<uint8_t> test;

    test.resize(width * rowHeight * bit);
    for (int dy = 0; dy < 3; dy++){
        for (int i = 0; i < width * rowHeight * bit; i++) test[i] = Pixels[i + dy * width * rowHeight * bit];
        e = GetEigenvalue(test, width, rowHeight, BitsPerPixel);
        cout << e << "\n";
        //  4%cri   4%cri   8%cri
        //  495040  495397  495703
        if(e == 495040 || e == 495397 || e == 495703) continue;

        minV = 999999;
        row = col = -1;
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 3; j++) {
                if(abs(table[i][j] - e) < min(minV, 500)){
                    minV = abs(table[i][j] - e);
                    row = i;
                    col = j;
                }
            }
        }
        if(row >= 0){
            if(col == 0) statistics[0][row]++;
            else statistics[1][row]++;
            if (row == 4) {
                for (int i = 0; i < 5; i++)
                attr[i] += 3;
            }
            else if(row == 9){
                attr[9] += 15;
            }
            else if(row == 10 || row == 11){
                attr[row] += 12;
            }
            else {
                attr[row] += col == 0 ? 6 : 3;
            }
        }
    }
    totalCube++;
    // imshow("Display w", Mat(rowHeight, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &test[0]));
    // waitKey(0);

    for (int i = 0; i < 12; i++) cout << attr[i] << " "; cout << "\n";
    for (int i = 0; i < 12; i++) cout << statistics[0][i] << " "; cout << "\n";
    for (int i = 0; i < 12; i++) cout << statistics[1][i] << " "; cout << "/ " << totalCube << "\n";

    if(attr[4] >= 6) return 4;
    anotherOK = (attr[8] >= 3) + ((attr[9] >= 15) || (attr[0] >= 3) || (attr[1] >= 3) || (attr[3] >= 3) || (attr[10] >= 12));
    if(attr[6] >=6 && anotherOK >= 2) return 6;
    anotherOK = (attr[8] >= 3) + ((attr[9] >= 15) || (attr[2] >= 3) || (attr[11] >= 12));
    if(attr[7] >=6 && anotherOK >= 2) return 7;
    for (int i = 0; i < 12; i++) if(attr[i] >= gate[i]) return i;
    return -1;
    // 1 0 2 7
}

int main(){
    vector<uint8_t> picture;
    int BitsPerPixel, width = 160, height = 14, posx = 600, posy = 427;
    int washX = 660, washY = 530;
    int bagX = 27, bagY = 90, dxy = 43;
    int consumeX = 50, comsumeY = 60, closeX = 750, closeY = 535;
    int cubeIdx, startIdx, washNum, washNumCount = 0, cubeX, cubeY, result;
    string resList[] = {"str", "dex", "int", "luk", "all", "hp", "ad", "ap", "adp"};
    bool enable = 0, isPause, isWeapon;

    set_key_value();
    cout<<"isWeapon >> isPause >> cubeIdx >> startIdx >> washNum \n";
    cin >> isWeapon >> isPause >> cubeIdx >> startIdx >> washNum;
    cubeX = bagX + dxy * (cubeIdx % 4);
    cubeY = bagY + dxy * (cubeIdx / 4);
    while(1){
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            for (int row = startIdx / 4; row < 6; row++) {
                for (int col = startIdx % 4; col < 4; col++) {
                    click(consumeX, comsumeY, "ALL");
                    syscontrol(400, &nowtime, &enable, &nowtick);
                    click(cubeX, cubeY, "ALL");
                    syscontrol(100, &nowtime, &enable, &nowtick);
                    click(cubeX, cubeY, "ALL");

                    click(bagX + dxy * col, bagY + dxy * row, "ALL");
                    syscontrol(200, &nowtime, &enable, &nowtick);
                    for (int i = 0; i < 4; i++) {
                    syscontrol(100, &nowtime, &enable, &nowtick);
                    keybd("ENTER", "ALL");
                    }
                    syscontrol(2000, &nowtime, &enable, &nowtick);

                    while(1){
                        ScreenShot(picture, BitsPerPixel, width, height * 3, posx, posy);
                        // imshow("Display w", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &cost[0]));
                        // waitKey(0);
                        result = checkfit(picture, width, height * 3, BitsPerPixel);
                        cout << startIdx << " " << result << "\n";
                        if (isWeapon ? (result == 6 || result == 7) : (result >= 0)){
                            washNumCount++;
                            startIdx++;
                            syscontrol(100, &nowtime, &enable, &nowtick);
                            cout << result << ", " << resList[result] << "\n";
                            if (isPause) {
                                keybd("F10", "ALL");
                                enable = 0;
                                system("pause");
                            }
                            else{
                                syscontrol(1500, &nowtime, &enable, &nowtick);
                            }
                            click(closeX, closeY, "ALL");
                            syscontrol(500, &nowtime, &enable, &nowtick);
                            if(washNum <= washNumCount) {
                                row = 999;
                                col = 999;
                            }
                            break;
                        }
                        else{
                            click(washX, washY, "ALL");
                            for (int i = 0; i<4; i++){
                                syscontrol(100, &nowtime, &enable, &nowtick);
                                keybd("ENTER", "ALL");
                            }
                            syscontrol(2000, &nowtime, &enable, &nowtick);
                        }
                    }
                }
            }
            enable = 0;
            cout << "done\n";
            system("pause");
        }
    }
    return 0;
}