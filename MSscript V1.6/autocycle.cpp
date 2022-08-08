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
    long int starttime = 0;
    int useT = 1;
    bool enable = 0;
    HWND nowWND, msWND = FindWindow(NULL, (LPCSTR)"MapleStory");
    set_key_value();
    
    while(1){
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            cout << useT++ << "\n";
            if (starttime == 0) {
                gettimeofday(&nowtime, NULL);
                starttime = nowtime.tv_sec * 1000 + nowtime.tv_usec / 1000;
            }
            nowWND = GetForegroundWindow();
            
            SetFocus(msWND);
            syscontrol(1000, &nowtime, &enable, &nowtick);
            keybd("DEL", "ALL");
            syscontrol(500, &nowtime, &enable, &nowtick);
            keybd("DEL", "ALL");
            syscontrol(500, &nowtime, &enable, &nowtick);
            SetFocus(nowWND);

            syscontrol(5*60*1000, &nowtime, &enable, &nowtick);
        }
    }

    return 0;
}