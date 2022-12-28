#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <map>
// #include <windows.h>
#include <ctime>

#include <cstring>
#include <vector>

#include "library/systemFnc.h"
#include "library/picture.h"


int main(){
    // 85~92 480~880
    const char* targetWnd = "¤Û¶ð¤â¹C  ";
    SYS scriptHT(targetWnd);
    PICTURE picHT(400, 70, targetWnd);
    std::cout << scriptHT.targetWNDsize.right << ", " << scriptHT.targetWNDsize.left << ", " << scriptHT.targetWNDsize.bottom << ", " << scriptHT.targetWNDsize.top << "\n";
    while(1){
        scriptHT.wait(200);
        std::cout << FindWindow(NULL, (LPCSTR)targetWnd) << ", " << GetForegroundWindow() << "\n";
        picHT.screenShot(480, 85).display();
    }
    system("pause");
    return 0;
}