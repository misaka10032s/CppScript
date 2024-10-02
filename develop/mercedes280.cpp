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

skilloption sakura("sakura", {"4", "4", "3"}, {500, 800, 500}, 100, 180 * 1000);
skilloption knight("knight", {"5", "5", "2"}, {500, 1200, 400}, 100, 180 * 1000);
skilloption eternal("eternal", {"6", "6"}, {500, 5000}, 100, 360 * 1000);
skilloption spider("spider", {"9", "9"}, {500, 800}, 100, 240 * 1000);
skilloption sun("sun", {"8", "8"}, {500, 800}, 100, 240 * 1000);
skilloption supernova("supernova", {"7", "7"}, {500, 400}, 100, 60 * 1000);
skilloption angryangel("angryangel", {"C", "C", "X", "RSHIFT", "RSHIFT"}, {100, 200, 120, 200, 450}, 100, 60 * 1000);
skilloption javelin("javelin", {"C", "C", "X", "PGD", "PGD"}, {100, 200, 120, 120, 900}, 100, 30 * 1000);
skilloption normal("normal", {"C", "C", "X"}, {100, 350, 400}, 100, 0);
skilloption normal2("normal2", {"C", "C", "X", "X", "X", "X"}, {100, 400, 150, 150, 150, 150}, 50, 30 * 1000);
skilloption moonlight("moonlight", {"C", "C", "X", "N", "N", "PGD", "PGD"}, {100, 450, 100, 150, 250, 120, 900}, 50, 40 * 1000);
skilloption moonlight2("moonlight2", {"C", "C", "X", "N", "N", "X", "X"}, {100, 450, 100, 150, 250, 120, 900}, 50, 34 * 1000);
skilloption cycle("cycle", {"DEL", "DEL"}, {300, 1000}, 100, 300 * 1000);

skilloption angryangelNo2Jump("angryangelNo2Jump", {"C", "X", "RSHIFT", "RSHIFT"}, {100, 120, 200, 450}, 100, 9 * 1000);
skilloption javelinNo2Jump("javejavelinNo2Jumplin", {"C", "X", "PGD", "PGD"}, {100, 80, 120, 1000}, 100, 7 * 1000);
skilloption normalNo2Jump("normalNo2Jump", {"C", "X"}, {120, 650}, 100, 0);
skilloption normal2No2Jump("normal2No2Jump", {"C", "X", "X", "X", "X"}, {100, 150, 150, 150, 150}, 50, 30 * 1000);
skilloption moonlightNo2Jump("moonlightNo2Jump", {"C", "X", "N", "N", "PGD", "PGD"}, {100, 450, 150, 250, 120, 900}, 50, 40 * 1000);
skilloption moonlight2No2Jump("moonlight2No2Jump", {"C", "X", "N", "N", "X", "X"}, {100, 450, 150, 250, 120, 900}, 50, 34 * 1000);

inline void upJump(SYS sys){
    sys.keybd("C", 3);
    sys.wait(50);
    sys.keybd("UP", 1);
    sys.wait(350);
    sys.keybd("C", 3);
        sys.wait(50);
    if(rand()%100 < 60){
        sys.keybd("X", 3);
        sys.wait(200);
    }
    sys.keybd("UP", 2);
    sys.wait(100);
    sys.keybd("PGD", 3);
    sys.wait(800);
}

inline void turnToTheRightWay(SYS sys, MSsetting ms){
    if(ms.direction == 1){
        sys.keybd("LEFT", 2);
        if(ms.hikikae[1] - ms.hikikae[0] < 10) sys.keybd("RIGHT", 3);
        else sys.keybd("RIGHT", 1);
    }
    else if(ms.direction == 0){
        sys.keybd("RIGHT", 2);
        if(ms.hikikae[1] - ms.hikikae[0] < 10) sys.keybd("LEFT", 3);
        else sys.keybd("LEFT", 1);
    }
}

// 35 48 61 73

// 38 61
// 69 48
// 109 48

int main(){
    const char* targetWnd = "MapleStory";
    SYS scriptMS(targetWnd);
    MSsetting infoMS({60, 90}, 999 * 90 * 1000, {0, 62}, {150, 80}, {cycle, spider, sun, knight, sakura, eternal, angryangelNo2Jump, javelinNo2Jump, normal2No2Jump, moonlightNo2Jump, moonlight2No2Jump, normalNo2Jump});
    PICTURE mapImg(infoMS.miniMapSize.x, infoMS.miniMapSize.y, targetWnd);
    long int nowtick, lastUpJump = 0;
    int anyway = 10000, upJumpCD = 60 * 1000;
    int TOPSTAIR = 48, FLOORSTAIR = 73;

    pointMS P1(38, 61), P2(69, 48), P3(109, 48);
    int cycleCD = 60 * 1000, lastCycle = 0, cycleStep = 0;

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
            for(int i=0; i<(int)infoMS.skills.size(); i++) if(infoMS.skills[i].lastuse == -1) infoMS.skills[i].lastuse = nowtick - infoMS.skills[i].cd - 100;

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
                        // infoMS.to = {anyway, FLOORSTAIR};
                        infoMS.now_action = 5;
                    }
                    else if(nowtick - lastCycle > cycleCD){
                        lastCycle = nowtick;
                        cycleStep = 1;
                        infoMS.to = P1;
                        infoMS.now_action = 4;
                    }
                    else if(infoMS.charpos.y > 0 && infoMS.charpos.y < TOPSTAIR){
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
                        upJump(scriptMS);
                    }
                    else{
                        infoMS.setDirection();
                        turnToTheRightWay(scriptMS, infoMS);
                        scriptMS.keybd("LEFT", 2);
                        scriptMS.keybd("RIGHT", 2);

                        if(nowtick - lastUpJump > upJumpCD && infoMS.charpos.y > infoMS.charStay.y){
                            lastUpJump = nowtick;
                            upJump(scriptMS);
                        }

                        for(int i=0; i<(int)infoMS.skills.size(); i++){
                            std::cout << nowtick << " " << infoMS.skills[i].skillname << " " << (nowtick - infoMS.skills[i].lastuse)/1000.0 << " " << infoMS.skills[i].cd/1000 << "\n";
                            if(nowtick - infoMS.skills[i].lastuse > infoMS.skills[i].cd){
                                std::cout << "skillname " << infoMS.skills[i].skillname << "\n---------------------------------------\n";
                                if ((infoMS.skills[i].skillname == "spider" || infoMS.skills[i].skillname == "supernava") && (infoMS.charpos.x < infoMS.hikikae[0] || infoMS.charpos.x > infoMS.hikikae[1])) continue;

                                else if (infoMS.skills[i].skillname == "knight"){
                                    // infoMS.skills[5] == sakura
                                    infoMS.skills[5].lastuse = MAX(nowtick - 121 * 1000, infoMS.skills[5].lastuse);
                                }

                                else if (infoMS.skills[i].skillname == "sakura"){
                                    // infoMS.skills[4] == knight
                                    infoMS.skills[4].lastuse = MAX(nowtick - 61 * 1000, infoMS.skills[4].lastuse);
                                }

                                infoMS.skills[i].lastuse = nowtick;
                                if(infoMS.skills[i].skillname != "sakura" && infoMS.skills[i].skillname != "knight" && infoMS.skills[i].skillname != "eternal"){
                                    infoMS.skills[i].lastuse += rand()%(infoMS.skills[i].cd / 20 + 1);
                                }

                                for (int j = 0; j < infoMS.skills[i].sknum; j++){
                                    getpos(mapImg, infoMS.charpos, infoMS.ringpos, infoMS.isOther);
                                    scriptMS.keybd(infoMS.skills[i].KBDname[j].data(), 3);
                                    scriptMS.wait(infoMS.skills[i].KBDdelay[j]);
                                }
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
                            if(infoMS.to.y > infoMS.charpos.y + 8) {
                                scriptMS.keybd("DOWN", 1);
                                scriptMS.wait(100);
                                scriptMS.keybd("C", 3);
                                scriptMS.wait(100);
                                scriptMS.keybd("C", 3);
                                scriptMS.wait(100);
                                scriptMS.keybd("DOWN", 2);
                                scriptMS.wait(500);
                            } else {
                                if((infoMS.to.x != anyway && infoMS.charpos.x > infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x > infoMS.hikikae[1])){
                                    infoMS.direction = 0;
                                }
                                else if((infoMS.to.x != anyway && infoMS.charpos.x < infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x < infoMS.hikikae[0])){
                                    infoMS.direction = 1;
                                }

                                turnToTheRightWay(scriptMS, infoMS);
                                
                                if(abs(infoMS.charpos.x - infoMS.to.x) < 50){
                                    upJump(scriptMS);
                                }
                                else{
                                    scriptMS.keybd("C", 3);
                                    scriptMS.wait(100);
                                    scriptMS.keybd("C", 3);
                                    scriptMS.wait(180);
                                    scriptMS.keybd("X", 3);
                                    if(rand()%100 < 60){
                                        scriptMS.wait(50);
                                        scriptMS.keybd("X", 3);
                                    }
                                    scriptMS.wait(200);
                                    scriptMS.keybd("UP", 2);
                                    scriptMS.wait(500);
                                }
                            }
                        }

                        getpos(mapImg, infoMS.charpos, infoMS.ringpos, infoMS.isOther);

                        if((infoMS.to.x != anyway && infoMS.charpos.x > infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x > infoMS.hikikae[1])){
                            infoMS.direction = 0;
                        }
                        else if((infoMS.to.x != anyway && infoMS.charpos.x < infoMS.to.x) || (infoMS.to.x == anyway && infoMS.charpos.x < infoMS.hikikae[0])){
                            infoMS.direction = 1;
                        }

                        if(infoMS.to.x != anyway) turnToTheRightWay(scriptMS, infoMS);

                        if(abs(infoMS.charpos.x - infoMS.to.x) > 35 && infoMS.to.x != anyway){
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
                        else if(cycleStep == 1) {
                            scriptMS.keybd("7", 3);
                            cycleStep = 2;
                            infoMS.to = P2;
                        }
                        else if(cycleStep == 2) {
                            scriptMS.keybd("F6", 3);
                            cycleStep = 3;
                            infoMS.to = P3;
                        }
                        else if(cycleStep == 3) {
                            scriptMS.keybd("F6", 3);
                            cycleStep = 0;
                            infoMS.now_action = 1;
                        }
                        else infoMS.now_action = 1;
                    }
                    break;
                case 5: // pick money
                    infoMS.lastpick = scriptMS.getNowtick();

                    infoMS.setDirection();
                    turnToTheRightWay(scriptMS, infoMS);

                    if(infoMS.charpos.y < FLOORSTAIR){
                        while(infoMS.charpos.y < FLOORSTAIR){
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
                    }
                    else {
                        upJump(scriptMS);
                    }
                    
                    if(infoMS.charpos.x < 90){
                        scriptMS.keybd("RIGHT", 1);
                    }
                    else{
                        scriptMS.keybd("LEFT", 1);
                    }

                    while(infoMS.charpos.x > infoMS.hikikae[0] && infoMS.charpos.x < infoMS.hikikae[1]){
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