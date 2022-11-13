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
    const char* targetWnd = "MapleStory";
    SYS scriptMS(targetWnd);
    MSsetting infoMS;
    PICTURE mapImg(targetWnd);
    long int nowtick;
    int anyway = 10000;

    int usedScriptId;
    if(getCodename()) {
        std::cout<<"please make scripts.\n";
        return 0;
    }
    std::cout<<"input script Id.\n";
    std::cin >> usedScriptId;
    readScript(usedScriptId, infoMS);
    mapImg.resize(infoMS.miniMapSize.x, infoMS.miniMapSize.y, 4);

    while(1){
        scriptMS.wait(50);
        if(scriptMS.isEnable()){
            std::cout<<"action: " << infoMS.now_action << "\n";
            if(infoMS.now_action) {
                getpos(mapImg, infoMS.charpos, infoMS.ringpos, infoMS.isOther);
            }

            if(infoMS.charpos.x == -1) infoMS.errorpos++;
            else infoMS.errorpos = 0;

            if(infoMS.errorpos > 50) {
                std::cout << "RIP\n";
                scriptMS.pause();
            }

            std::cout << "char pos: " << infoMS.charpos.x << " " << infoMS.charpos.y << " ring pos: " << infoMS.ringpos.x << " " << infoMS.ringpos.y
            << " togo: " << infoMS.to.x << " " << infoMS.to.y << " dir: " << infoMS.direction << "\n";
            infoMS.setDirection();

            nowtick = scriptMS.getNowtick();
            if(infoMS.lastpick == -1) infoMS.lastpick = nowtick;
            if(infoMS.lastring == -1) infoMS.lastring = nowtick - infoMS.ringCD;

            switch (infoMS.now_action){
                case 0:
                    std::cout << "stop\n";
                    break;

                case 1:
                    if(FindWindow(NULL, (LPCSTR)targetWnd) == NULL){
                        infoMS.now_action = 0;
                    }
                    else if(infoMS.ringpos.x != -1 && (nowtick - infoMS.lastring > infoMS.ringCD || nowtick - infoMS.lastring < 20000)){
                        infoMS.to = infoMS.ringpos;
                        infoMS.isring = 1;
                        infoMS.now_action = 4;
                    }
                    else if(nowtick - infoMS.lastpick > infoMS.pickCD){
                        infoMS.to = {anyway, 60};
                        infoMS.now_action = 4;
                    }
                    else if(infoMS.charpos.y > 0 && infoMS.charpos.y < infoMS.charStay.y - 12){
                        scriptMS.keybd("DOWN", 1);
                        scriptMS.wait(100);
                        scriptMS.keybd("C", 3);
                        scriptMS.wait(100);
                        scriptMS.keybd("C", 3);
                        scriptMS.wait(50);
                        scriptMS.keybd("DOWN", 2);
                        scriptMS.wait(200);
                        scriptMS.keybd("X", 3);
                        scriptMS.wait(400);
                    }
                    else if(infoMS.charpos.y > infoMS.charStay.y + 3){
                        infoMS.to = {anyway, infoMS.charStay.y};
                        infoMS.now_action = 4;
                    }
                    else{
                        infoMS.setDirection();
                        
                        if(infoMS.direction == 1){
                            scriptMS.keybd("LEFT", 2);
                            scriptMS.keybd("RIGHT", 1);
                        }
                        else if(infoMS.direction == 0){
                            scriptMS.keybd("RIGHT", 2);
                            scriptMS.keybd("LEFT", 1);
                        }

                        for(int i=0; i<(int)infoMS.skills.size(); i++){
                            std::cout << nowtick << " " << infoMS.skills[i].skillname << " " << infoMS.skills[i].lastuse << "\n";
                            if(infoMS.skills[i].canUse(nowtick)){
                                actSkill(infoMS.skills[i], scriptMS, infoMS);
                                break;
                            }
                        }
                    }
                    break;

                case 2: // solve ring
                    scriptMS.keybd("Y", 3);
                    scriptMS.wait(400);
                    solveWheel(scriptMS);
                    infoMS.lastring = nowtick;
                    infoMS.now_action = 1;
                    infoMS.isring = 0;
                    break;

                case 4:
                    if((abs(infoMS.charpos.x - infoMS.to.x) < 5 || infoMS.to.x == anyway) && abs(infoMS.charpos.y - infoMS.to.y) < 4) {
                        infoMS.timecount++;
                        scriptMS.keybd("RIGHT", 2);
                        scriptMS.keybd("LEFT", 2);
                    }
                    else{
                        infoMS.timecount = 0;

                        if(abs(infoMS.to.y - infoMS.charpos.y) > 3){
                            if(infoMS.to.y > infoMS.charpos.y + 8){
                                actSkill((std::string)"downfloor", scriptMS, infoMS);
                            }
                            else{
                                if((infoMS.to.x != anyway && infoMS.charpos.x > infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x > infoMS.hikikae[1])){
                                    infoMS.direction = 0;
                                }
                                else if((infoMS.to.x != anyway && infoMS.charpos.x < infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x < infoMS.hikikae[0])){
                                    infoMS.direction = 1;
                                }

                                if(infoMS.direction == 1){
                                    scriptMS.keybd("LEFT", 2);
                                    scriptMS.keybd("RIGHT", 1);
                                }
                                else if(infoMS.direction == 0){
                                    scriptMS.keybd("RIGHT", 2);
                                    scriptMS.keybd("LEFT", 1);
                                }
                                scriptMS.keybd("C", 3);
                                scriptMS.wait(100);
                                scriptMS.keybd("UP", 1);
                                scriptMS.wait(300);
                                scriptMS.keybd("C", 3);
                                scriptMS.wait(200);
                                scriptMS.keybd("UP", 2);
                                scriptMS.wait(500);
                            }
                        }

                        getpos(mapImg, infoMS.charpos, infoMS.ringpos, infoMS.isOther);

                        if((infoMS.to.x != anyway && infoMS.charpos.x > infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x > infoMS.hikikae[1])){
                            infoMS.direction = 0;
                        }
                        else if((infoMS.to.x != anyway && infoMS.charpos.x < infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x < infoMS.hikikae[0])){
                            infoMS.direction = 1;
                        }

                        if(infoMS.to.x != anyway){
                            if(infoMS.direction == 1){
                                scriptMS.keybd("LEFT", 2);
                                scriptMS.keybd("RIGHT", 1);
                            }
                            else if(infoMS.direction == 0){
                                scriptMS.keybd("RIGHT", 2);
                                scriptMS.keybd("LEFT", 1);
                            }
                        }

                        if(abs(infoMS.charpos.x - infoMS.to.x) > 35 && infoMS.to.x != anyway){
                            scriptMS.keybd("C", 1);
                            scriptMS.wait(100);
                            scriptMS.keybd("C", 3);
                            scriptMS.wait(100);
                            scriptMS.keybd("C", 3);
                            scriptMS.wait(150);
                        }
                        else{
                            scriptMS.wait(300);
                        }
                        
                        scriptMS.keybd("RIGHT", 2);
                        scriptMS.keybd("LEFT", 2);
                    }
                    if(infoMS.isring && infoMS.ringpos.x == infoMS.ringpos.y && infoMS.ringpos.x == -1){
                        infoMS.now_action = 2;
                        break;
                    }

                    if( infoMS.timecount > 0){
                        infoMS.timecount = 0;
                        infoMS.to.x = anyway;
                        if(infoMS.isring) infoMS.now_action = 2;
                        else if(nowtick - infoMS.lastpick > infoMS.pickCD) infoMS.now_action = 5;
                        else infoMS.now_action = 1;
                    }
                    break;
                case 5: // pick money
                    infoMS.lastpick = scriptMS.getNowtick();

                    while(infoMS.charpos.y < 55){
                        scriptMS.keybd("DOWN", 1);
                        scriptMS.wait(50);
                        scriptMS.keybd("C", 3);
                        scriptMS.wait(50);
                        scriptMS.keybd("C", 3);
                        scriptMS.wait(50);
                        scriptMS.keybd("DOWN", 2);
                        scriptMS.wait(430);
                        getpos(mapImg, infoMS.charpos, infoMS.ringpos, infoMS.isOther);
                    }
                    
                    if(infoMS.charpos.x < 90){
                        scriptMS.keybd("RIGHT", 1);
                    }
                    else{
                        scriptMS.keybd("LEFT", 1);
                    }

                    while(infoMS.charpos.x > 30 && infoMS.charpos.x < 140){
                        scriptMS.keybd("C", 3);
                        scriptMS.wait(160);
                        scriptMS.keybd("C", 3);
                        scriptMS.wait(250);
                        scriptMS.keybd("X", 3);
                        scriptMS.wait(680);
                        getpos(mapImg, infoMS.charpos, infoMS.ringpos, infoMS.isOther);
                    }

                    scriptMS.keybd("RIGHT", 2);
                    scriptMS.keybd("LEFT", 2);
                    infoMS.now_action = 1;
                    break;
            }
        }
    }

    return 0;
}