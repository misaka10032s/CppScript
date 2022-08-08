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
    vector<uint8_t> picture;
    int BitsPerPixel, width = 160, height = 15, posx = 605, posy = 408;
    bool showDetail;

    cin >> showDetail;
    set_key_value();
    while(1){
        cout << "posx, posy, width, height\n";
        cin >> posx >> posy >> width >> height;
        ScreenShot(picture, BitsPerPixel, width, height, posx, posy);
        cout << GetEigenvalue(picture, width, height, BitsPerPixel) << "\n";
        if(showDetail)
            for (int i = 0; i < width * height * BitsPerPixel / 8; i++)
                cout << (int)picture[i] << " ";
        imshow("Display w", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &picture[0]));
        waitKey(0);
    }
    return 0;
}