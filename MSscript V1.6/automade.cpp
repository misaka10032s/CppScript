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

#include "imgproc.cpp"
// #include "KBDscript.cpp"
#include "skillset.cpp"
#include "sysfunc.cpp"

using namespace cv;
using namespace std;

#define WIN32_LEAN_AND_MEAN

int main(){
    int makeX = 536, makeY = 353;
    int Xs[11] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, Ys[11] = {185, 202, 219, 236, 253, 271, 290, 305, 322, 340, 355};
    int type, cd, times, t = 1, cdDx;
    double eatnum, eatCounter = 0;
    bool enable = 0;
    struct timeval timestamp;
    set_key_value();
    cout << "type cd(min) times eatnum\n";
    cin >> type >> cd >> times >> eatnum;

    while(1){
        syscontrol(20 + (enable ? 0 : 30), &timestamp, &enable, &nowtick);
        if(enable){
            eatCounter += eatnum;
            cout<<t++<<" "<<eatCounter<<"\n";
            switch (type){
            case 1:
                cdDx = 1000;
                click(makeX, makeY, "ALL");
                syscontrol(400, &timestamp, &enable, &nowtick);
                keybd("ENTER", "ALL");
                syscontrol(3000, &timestamp, &enable, &nowtick);
                break;

            case 2:
                cdDx = -19000;
                for (int i = 0; i < 8; i++){
                    click(Xs[i], Ys[i], "ALL");
                    syscontrol(400, &timestamp, &enable, &nowtick);
                    click(makeX, makeY, "ALL");
                    syscontrol(400, &timestamp, &enable, &nowtick);
                    keybd("ENTER", "ALL");
                    syscontrol(3000, &timestamp, &enable, &nowtick);
                }
                break;

            case 3:
                cdDx = -3000;
                for (int i = 0; i < 2; i++){
                    click(Xs[i], Ys[i], "ALL");
                    syscontrol(400, &timestamp, &enable, &nowtick);
                    click(makeX, makeY, "ALL");
                    syscontrol(400, &timestamp, &enable, &nowtick);
                    keybd("ENTER", "ALL");
                    syscontrol(3000, &timestamp, &enable, &nowtick);
                }
                break;

            default:
                enable = 0;
                cout << "error\n";
                break;
            }

            if(eatCounter >= 1) {
                click(makeX + 150, makeY, "ALL");
            }
            while (eatCounter >= 1){
                keybd("INS", "ALL");
                syscontrol(1000, &timestamp, &enable, &nowtick);
                cdDx -= 1000;
                eatCounter--;
            }
            syscontrol(cd * 60000 + cdDx, &timestamp, &enable, &nowtick);

            if(t>times){
                // enable = 0;
                cout << "finished\n";
                keybd("=", "ALL");
                syscontrol(1000, &timestamp, &enable, &nowtick);
                keybd("F10", "ALL");
                enable = 0;
            }
        }
    }
}