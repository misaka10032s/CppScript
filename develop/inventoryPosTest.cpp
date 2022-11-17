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
    pointMS ps;
    const char* targetWnd = "MapleStory";
    SYS scriptMS(targetWnd);

    scriptMS.wait(200);
    for(int i=0; i<4; i++){
        for(int j=-6; j<24; j++){
            ps = getInventoryPos(j, 0, i);
            scriptMS.mouseMV(ps.x, ps.y);
            scriptMS.wait(200);
        }
        scriptMS.pause();
    }
    return 0;
}