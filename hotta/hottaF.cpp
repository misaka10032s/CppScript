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

int main(){
    bool enable = 0;
    set_key_value();

    while (1) {
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            syscontrol(1*1000, &nowtime, &enable, &nowtick);
            keybd("F", "ALL");
            cout << "F\n";
        }
    }
    return 0;
}