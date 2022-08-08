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
    int craftX = 226, craftY = 253;
    int resaultX = 745, resaultY = 253;
    int inventoryX = 805, inventoryY = 467;
    bool enable = 0;

    set_key_value();

    while (1) {
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            keybd("LSHIFT", "DOWN");
            syscontrol(100, &nowtime, &enable, &nowtick);
            clickLeft(craftX, craftY, "ALL");
            syscontrol(200, &nowtime, &enable, &nowtick);
            clickLeft(resaultX, resaultY, "ALL");
            syscontrol(500, &nowtime, &enable, &nowtick);
            clickLeft(craftX, craftY, "ALL");
            syscontrol(200, &nowtime, &enable, &nowtick);
            keybd("LSHIFT", "UP");

            for(int i=0; i<75; i++){
                clickLeft(resaultX, resaultY, "ALL");
                syscontrol(25, &nowtime, &enable, &nowtick);
            }

            syscontrol(400, &nowtime, &enable, &nowtick);
            clickLeft(inventoryX, inventoryY, "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);
        }
    }
    return 0;
}