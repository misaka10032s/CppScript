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

#include "library/imgproc.cpp"
#include "library/sysfunc.cpp"

using namespace cv;
using namespace std;

#define WIN32_LEAN_AND_MEAN

void SolveWheel(){
    std::vector<uint8_t> puzz;
    string filename = gettimestring();
    map<int, const char *> UDLR;
    UDLR[2] = "DOWN";
    UDLR[4] = "LEFT";
    UDLR[6] = "RIGHT";
    UDLR[8] = "UP";
    int BitsPerPixel, width = 500, height = 150, x = 450, y = 130, way[4] = {2,6,4,8};
    ScreenShot(puzz, BitsPerPixel, width, height, x, y);
    Sleep(100);
    for (int i = 0; i < 4;i++) {
        filename += (string)" " + UDLR[way[i]];
    }
    imwrite((string) "ringRECORD/" + filename + (string) ".jpg", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &puzz[0]));
}

int main(){
    SolveWheel();
    return 0;
}