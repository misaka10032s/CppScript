#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <opencv2/opencv.hpp>
// #include <cstdint>
#include <cstring>
#include <vector>
// #include <tiffio.h>
#include <string>
// #include <sstream>
// #include <dirent.h>

#include "systemOS.h"


#ifndef LIB_PICTURE_H_
#define LIB_PICTURE_H_

#ifdef SYS_LINUX
#endif
#ifdef SYS_WINDOWS
#include <windows.h>
#endif

class RANGE{
    public:
        RANGE();
        RANGE(int L, int R, int T, int B);
        RANGE(std::initializer_list<int> POS);

        void retract(RANGE range);
        void regularize();

        int left, right, top, bottom;
};

class PICTURE{
    public:
        PICTURE();
        PICTURE(const char* window);
        PICTURE(int width, int height);
        PICTURE(int width, int height, const char* window);

        std::vector<uint8_t> Pixels;
        int width, height, bit;
        
        #ifdef SYS_LINUX
        std::string hwnd;
        #endif
        #ifdef SYS_WINDOWS
        HWND hwnd;
        #endif
        
        void display();
        void display(std::string windowName);
        PICTURE resize(int width, int height, int bit);
        PICTURE setWindow(char* window);

        bool open(std::string fileName);
        PICTURE screenShot(int dx, int dy);
        PICTURE twoValue(PICTURE &targetPic, int gate);
        PICTURE GaussBlur(PICTURE &targetPic, double rate);
        PICTURE twist(PICTURE &targetPic, int posx, int posy, double rate);
        PICTURE blackhole(PICTURE &targetPic, int posx, int posy, double gravity, double factor);
        PICTURE negative(PICTURE &targetPic);
        PICTURE RGBtoYUV(PICTURE &targetPic, char type);
        PICTURE GammaTransform(PICTURE &targetPic, double gamma);
        PICTURE jpeg_compress(PICTURE &targetPic, double Q);
        PICTURE jpeg_decompress(PICTURE &targetPic, double Q);
        PICTURE sharp(PICTURE &targetPic, double rate);
        PICTURE imgFloor(PICTURE &targetPic, int flr);
        PICTURE matrixTrans(PICTURE &targetPic, int posx, int posy, double mat[4], RANGE range);
        PICTURE rotateD(PICTURE &targetPic, int posx, int posy, double deg, RANGE range);
        PICTURE rotateQ(PICTURE &targetPic, int qNum);
        PICTURE ripple(PICTURE &targetPic, int posx, int posy, double density);
        PICTURE subPic(PICTURE &targetPic, RANGE range);
        PICTURE copyPaste(PICTURE &targetPic, RANGE oRange, int pX, int pY);
        PICTURE rgbMatrixTrans(PICTURE &targetPic, int mtx[9]);
};
#endif