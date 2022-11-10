#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <map>
// #include <windows.h>
#include <ctime>

#include <cstring>
#include <vector>

#include "library/systemFnc.h"
#include "library/maplestory.h"

int main(){
    pointMS mapSize, charPos, ringPos;
    bool isOther;
    const char* targetWnd = "MapleStory";
    SYS scriptMS(targetWnd);
    MSsetting infoMS;
    PICTURE mapImg(targetWnd);

    std::cout << "input map width and height";
    std::cin >> mapSize.x >> mapSize.y;
    mapImg.resize(mapSize.x, mapSize.y, 4);
    while(1){
        scriptMS.wait(100);
        getpos(mapImg, charPos, ringPos, isOther);
        std::cout << charPos.x << ", " << charPos.y << "\n";
    }
    return 0;
}