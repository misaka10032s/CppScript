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

int main(){
    vector<uint8_t> picture, origPixels;
    string filename;
    Mat origImg;
    int origWidth, origHeight, origBpp;
    int bit, BitsPerPixel, width = 160, height = 14, posx = 600, posy = 427;

    set_key_value();
    while(1){
        cin >> filename;
        origImg = imread("sample/" + filename + ".jpg");
        origWidth = origImg.cols;
        origHeight = origImg.rows;
        origBpp = origImg.channels() * 8;
        origPixels.resize(origWidth * origHeight * origImg.channels());
        origPixels.assign(origImg.data, origImg.data + origImg.total()*origImg.channels());

        BitsPerPixel = origBpp;
        bit = origBpp > 24 ? 4 : 3;

        picture.resize(width * height * bit);
        for (int dy = 0; dy < 3; dy++){
            for (int x = 0; x < width; x++){
                for (int y = 0; y < height; y++){
                    for (int k = 0; k < bit; k++){
                        picture[x * bit + y * width * bit + k] = origPixels[(posx + x) * bit + (dy*height + posy + y) * origWidth * bit + k];
                    }
                }
            }
            cout << GetEigenvalue(picture, width, height, BitsPerPixel) << "\n";
            imshow("Display w", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &picture[0]));
            waitKey(0);
        }
    }
    return 0;
}