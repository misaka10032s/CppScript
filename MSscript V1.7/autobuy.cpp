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
    int finT = 1,exeTime;
    bool enable = 0;
    set_key_value();

    cout << "input exeTime:\n";
    cin >> exeTime;
    
    while(1){
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            cout << finT++ << "\n";
            if (LOWORD(GetKeyState(VK_NUMLOCK))) {
                keybd("NUMLOCK", "ALL");
                syscontrol(50, &nowtime, &enable, &nowtick);
            }
            keybd("Y", "ALL");
            syscontrol(100, &nowtime, &enable, &nowtick);
            keybd("Y", "ALL");
            syscontrol(100, &nowtime, &enable, &nowtick);
            for (int i = 0; i < 14;i++){
                keybd("DOWN", "ALL");
                syscontrol(80, &nowtime, &enable, &nowtick);
            }
            keybd("Y", "ALL");
            syscontrol(100, &nowtime, &enable, &nowtick);
            keybd("RIGHT", "ALL");
            syscontrol(100, &nowtime, &enable, &nowtick);
            keybd("ENTER", "ALL");
            syscontrol(100, &nowtime, &enable, &nowtick);
            keybd("ENTER", "ALL");
            syscontrol(100, &nowtime, &enable, &nowtick);

            if(finT > exeTime){
                enable = 0;
                cout << "finish "<< exeTime <<" times\n";
                if (!LOWORD(GetKeyState(VK_NUMLOCK))) {
                    keybd("NUMLOCK", "ALL");
                    syscontrol(50, &nowtime, &enable, &nowtick);
                }
                // keybd("F10", "ALL");
                break;
            }
        }
    }

    return 0;
}