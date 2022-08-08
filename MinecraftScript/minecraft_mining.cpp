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

void openMenu(bool& enable){
    keybd("/", "ALL");
    syscontrol(400, &nowtime, &enable, &nowtick);
    keybd("M", "ALL");
    syscontrol(400, &nowtime, &enable, &nowtick);
    keybd("E", "ALL");
    syscontrol(400, &nowtime, &enable, &nowtick);
    keybd("N", "ALL");
    syscontrol(400, &nowtime, &enable, &nowtick);
    keybd("U", "ALL");
    syscontrol(400, &nowtime, &enable, &nowtick);
    keybd("ENTER", "ALL");
    syscontrol(400, &nowtime, &enable, &nowtick);
}

int main(){
    int length = 36, posx = 339, posy = 325, tmpX, tmpY;
    int channelX = 396, channelY = 160;
    int homeX = 573, homeY = 233;
    int townX = 428, townY = 233;
    int spaceX = 820, spaceY = 345;
    bool enable = 0;
    int duration;

    cin >> duration;
    duration = MAX(MIN(duration, 5), 300);
    // Sleep(2000);
    // SetWindowTextA(GetForegroundWindow(), (LPCSTR) "Minecraft");
    // char wnd_title[256];
    // cout << (GetActiveWindowTitle().substr(0, 9) == (string) "Minecraft") << "\n";
    // HWND hwnd = GetForegroundWindow();
    // GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
    // cout << wnd_title << "\n";
    // cout << FindWindow(NULL, (LPCSTR) "Minecraft") << "\n";
    // cout<<FindWindow(NULL, (LPCSTR)"MapleStory")<<"\n";
    // cout<<FindWindow(NULL, (LPCSTR)wnd_title)<<"\n";
    set_key_value();

    while (1) {
        syscontrol(50, &nowtime, &enable, &nowtick);
        if(enable){
            keybd("I", "ALL");
            syscontrol(duration * 60 * 1000, &nowtime, &enable, &nowtick);
            // syscontrol(5 * 1000, &nowtime, &enable, &nowtick);
            keybd("I", "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);

            openMenu(enable);

            clickLeft(channelX, channelY, "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);
            clickLeft(homeX, homeY, "ALL");
            syscontrol(5000, &nowtime, &enable, &nowtick);
            keybd("E", "ALL");

            for(int i=0; i<27; i++){
                tmpX = posx + (i%9)*length;
                tmpY = posy + (i/9)*length;
                clickLeft(tmpX + length/2, tmpY + length/2, "ALL");
                syscontrol(200, &nowtime, &enable, &nowtick);

                clickRight(tmpX + length/2, tmpY + length/2, "ALL");
                syscontrol(200, &nowtime, &enable, &nowtick);

                clickLeft(spaceX, spaceY, "ALL");
                syscontrol(200, &nowtime, &enable, &nowtick);
            }
            keybd("ESC", "ALL");

            syscontrol(200, &nowtime, &enable, &nowtick);
            openMenu(enable);
            clickLeft(channelX, channelY, "ALL");
            syscontrol(400, &nowtime, &enable, &nowtick);
            clickLeft(townX, townY, "ALL");
            syscontrol(5000, &nowtime, &enable, &nowtick);
        }
    }
    return 0;
}