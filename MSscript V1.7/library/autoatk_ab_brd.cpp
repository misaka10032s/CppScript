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
#include "KBDscript.cpp"
#include "skillset_ab.cpp"
#include "sysfunc.cpp"

using namespace cv;
using namespace std;

#define WIN32_LEAN_AND_MEAN


void FindPos(int *charposX, int *charposY, int *ringposX, int *ringposY, bool *isOther){
    std::vector<uint8_t> map;
    int BitsPerPixel, width = 175, height = 82, posx = 5, posy = 18;
    int charpos[2], ringpos[2];
    // while(1){
        ScreenShot(map, BitsPerPixel, width, height, posx, posy);
        // imshow("Display posres", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &map[0]));
        // waitKey(0);
        getpos(map, width, height, BitsPerPixel, charpos, ringpos, isOther);

        *charposX = charpos[0];
        *charposY = charpos[1];
        *ringposX = ringpos[0];
        *ringposY = ringpos[1];

        Sleep(40);
    // }
}

void FindBoss(bool *ifboss){
    std::vector<uint8_t> map, aftermap;
    int BitsPerPixel, width = 435, height = 70, x = 475, y = 350;
    int HPwidth = 200, HPheight = 40, HPx = 175, HPy = 5;
    int origE, edgeE, standard_origE = 12347362, standard_edgeE = 4392536; // 4392536 4535680
    int standard_HPorigE = 12347362, standard_HPedgeE = 4535680; //@@@@ 
    if(!(*ifboss)){
        ScreenShot(map, BitsPerPixel, width, height, x, y);
        showedge(map, aftermap, width, height, BitsPerPixel);
        origE = GetEigenvalue(map, width, height, BitsPerPixel);
        edgeE = GetEigenvalue(aftermap, width, height, BitsPerPixel);
        if(abs(standard_origE - origE) < 300000 && abs(standard_edgeE - edgeE) < 100000) (*ifboss) = 1;
        Sleep(40);
    }
    else{
        ScreenShot(map, BitsPerPixel, HPwidth, HPheight, HPx, HPy);
        showedge(map, aftermap, HPwidth, HPheight, BitsPerPixel);
        origE = GetEigenvalue(map, HPwidth, HPheight, BitsPerPixel);
        edgeE = GetEigenvalue(aftermap, HPwidth, HPheight, BitsPerPixel);
        if(abs(standard_HPorigE - origE) < 300000 && abs(standard_HPedgeE - edgeE) < 100000) (*ifboss) = 0;
    }
}

int findclosestladder(int charX, int charY){
    int ladderpos[9] = {24, 65, 111, 148, 47, 86, 125, 56, 116};
    int closestladderpos;
    // 第一層梯子x座標 24 65 111. 148.
    // 第二層梯子x座標 47 86 125
    // 第三層梯子x座標 56 116

    if(charY > 47){
        closestladderpos = 24;
        for (int i = 0; i < 4;i++){
            if(abs(ladderpos[i] - charX) < abs(closestladderpos - charX)){
                closestladderpos = ladderpos[i];
            }
        }
    }
    else if(charY > 33){
        closestladderpos = 47;
        for (int i = 4; i < 7;i++){
            if(abs(ladderpos[i] - charX) < abs(closestladderpos - charX)){
                closestladderpos = ladderpos[i];
            }
        }
    }
    else{
        closestladderpos = 56;
        for (int i = 7; i < 9;i++){
            if(abs(ladderpos[i] - charX) < abs(closestladderpos - charX)){
                closestladderpos = ladderpos[i];
            }
        }
    }
    return closestladderpos;
}

void SolveWheel(){
    std::vector<uint8_t> puzz;
    string filename = gettimestring();
    map<int, const char *> UDLR;
    UDLR[2] = "DOWN";
    UDLR[4] = "LEFT";
    UDLR[6] = "RIGHT";
    UDLR[8] = "UP";
    int BitsPerPixel, width = 500, height = 150, x = 450, y = 130, way[4];
    for (int i = 0; i < 4;i++) way[i] = 0;
    ScreenShot(puzz, BitsPerPixel, width, height, x, y);
    matchway(puzz, width, height, BitsPerPixel, way);
    if(way[3] == 0) return;
    Sleep(100);
    for (int i = 0; i < 4;i++){
        keybd(UDLR[way[i]], "ALL");
        Sleep(200);
    }
    Sleep(30);
    for (int i = 0; i < 4;i++) {
        filename += (string)" " + UDLR[way[i]];
    }
    imwrite((string)"ringRECORD/" + filename + (string)".jpg", Mat(height, width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &puzz[0]));
}

void Restart(){

}

void ScriptSystem(struct timeval *timestamp, int *charposX, int *charposY, int *ringposX, int *ringposY, bool *ifboss, bool *isOther){
    int now_action = 1; // 0 = starting game, 1 = normal, 2 = solve wheel, 3 = special boss, 4 = move to, 5 = pick money, 6 = buff
    int direction = -1; //direction 0: left 1: right -1: undefined
    int toX = -1, toY = -1, hikikae[2] = {55, 115}/*, throwidx, dfthrowidx*/;
    // int ItemX = 1200, ItemY = 180;
    int closestladderposX, timecount = 0, isOthertimecount = 0, errorpos, anyway = 1000;
    int pickCD = 3 * 60 * 1000, ringCD = 15 * 60 * 1000 + 5000/*, throwCD = 10 * 60 * 1000*/;
    long int /*nowtick,*/ lastpick = -1, lastbuff = -1, lastring = -1, lastthrow = -1/*, throwDelayCount = 0*/;
    bool enable = 0, isring = 0;
    // 5層的Y座標 60 47 33 25 18
    map<string, struct skilloption> skills;

    skills["01buff"] = buff;
    skills["02spotlight"] = spotlight;
    skills["03ribbon"] = ribbon;
    skills["04blackhole"] = blackhole;
    skills["10normal"] = normal;
    // cout << "input dfthrowidx:\n";
    // cin >> dfthrowidx;
    // dfthrowidx = MIN(MAX(dfthrowidx, 0), 23);
    while(1){
        syscontrol(20 + (enable ? 0 : 30), timestamp, &enable, &nowtick);

        if(enable){
            if (LOWORD(GetKeyState(VK_NUMLOCK))) {
                keybd("NUMLOCK", "ALL");
                syscontrol(50, timestamp, &enable, &nowtick);
            }
            cout<<"action: " << now_action << "\n";
            if(now_action) FindPos(charposX, charposY, ringposX, ringposY, isOther);
            if((*charposX) == -1){
                errorpos++;
            }
            else{
                errorpos = 0;
            }
            if(errorpos > 50){
                enable = 0;
                cout << "RIP\n";
            }

            if((*isOther)){
                isOthertimecount++;
            }
            else{
                if(isOthertimecount){
                    // skills["00cycle"].lastuse = nowtick - skills["00cycle"].cd + 1000;
                }
                isOthertimecount = 0;
            }
            if(isOthertimecount > 50){
                cout << "Warning\n";
            }
            
            // FindBoss(ifboss);
            cout << "char pos: " << (*charposX) << " " << (*charposY) << " ring pos: " << (*ringposX) << " " << (*ringposY);
            cout << " togo: " << toX << " " << toY << " dir: " << direction << "\n";
            if(direction == -1) direction = (*charposX) > 90 ? 0 : 1;
            
            // nowtick = (*timestamp).tv_sec * 1000 + (*timestamp).tv_usec / 1000;
            if(lastpick == -1) lastpick = nowtick;
            if(lastbuff == -1) lastbuff = nowtick - skills["buff"].cd;
            if(lastring == -1) lastring = nowtick - ringCD;
            if(lastthrow == -1) lastthrow = nowtick - (rand()%600) * 1000;
            for(auto &[key, val] : skills){
                if(val.lastuse == -1){
                    if(val.skillname == "buff" || val.skillname == "knight" || val.skillname == "arrow" || val.skillname == "cycle" || val.skillname == "spider" || val.skillname == "supernova") val.lastuse = nowtick - val.cd;
                    else val.lastuse = nowtick;
                }
                else{
                    // val.lastuse = nowtick - val.lastuse;
                }
            }

            switch (now_action){
            case 0:
                enable = 0;
                cout << "stop\n";
                break;
            
            case 1:
                if(FindWindow(NULL, (LPCSTR)"MapleStory") == NULL){
                    now_action = 0;
                }
                else if((*ringposX) != -1 && (nowtick - lastring > ringCD || nowtick - lastring < 20000)){
                    toX = (*ringposX);
                    toY = (*ringposY);
                    isring = 1;
                    now_action = 4;
                }
                else if((*ifboss)){
                    toX = (*charposX) + pow(-1, direction) * 15;
                    toY = (*charposY);
                    now_action = 4;
                }
                else if(nowtick - lastpick > pickCD){
                    toX = anyway;
                    toY = 60;
                    now_action = 4;
                }
                else if(nowtick - lastbuff > skills["buff"].cd){
                    now_action = 6;
                }
                else if((*charposY) > 0 && (*charposY) < 35){
                    keybd("DOWN", "DOWN");
                    syscontrol(100, timestamp, &enable, &nowtick);
                    keybd("C", "ALL");
                    syscontrol(100, timestamp, &enable, &nowtick);
                    keybd("C", "ALL");
                    syscontrol(50, timestamp, &enable, &nowtick);
                    keybd("DOWN", "UP");
                    syscontrol(200, timestamp, &enable, &nowtick);
                    keybd("X", "ALL");
                    syscontrol(400, timestamp, &enable, &nowtick);
                }
                else if((*charposY) > 50){
                    toX = anyway;
                    toY = 47;
                    now_action = 4;
                }
                else{
                    /*if(nowtick - lastthrow > throwCD){
                        if(throwidx > 24) {
                            throwidx = dfthrowidx;
                            lastthrow = nowtick - rand()%600 * 1000;
                        }
                        if(nowtick - throwDelayCount > 5000){
                            click(ItemX + (throwidx % 4) * 43, ItemY + (throwidx / 4) * 43, "ALL");
                            syscontrol(400, timestamp, &enable, &nowtick);
                            click(450 + rand()%100, 450 + rand()%100, "ALL");
                            throwDelayCount = nowtick - rand()%3000;
                        }
                    }*/
                    if((*charposX) > hikikae[1]){
                        direction = 0;
                    }
                    else if((*charposX) < hikikae[0]){
                        direction = 1;
                    }
                    if(direction == 1){
                        keybd("LEFT", "UP");
                        keybd("RIGHT", "DOWN");
                    }
                    else if(direction == 0){
                        keybd("RIGHT", "UP");
                        keybd("LEFT", "DOWN");
                    }
                    for(auto &[keye, val] : skills){
                    // for (int i = 0; i < loopskillnum;i++){
                    //     auto val = skills[skilltmp];
                    cout << nowtick << " " << val.skillname << " " << val.lastuse << "\n";
                    if (nowtick - val.lastuse > val.cd) {
                        cout << "skillname " << val.skillname << "\n---------------------------------------\n";
                        if ((val.skillname == "spider" || val.skillname == "supernava") && ((*charposX) < hikikae[0] || (*charposX) > hikikae[1]))
                            continue;

                        else if (val.skillname == "knight")
                        {
                            skills["05sakura"].lastuse = MAX(nowtick - 101 * 1000, skills["05sakura"].lastuse);
                        }

                        else if (val.skillname == "sakura")
                        {
                            skills["04knight"].lastuse = MAX(nowtick - 61 * 1000, skills["04knight"].lastuse);
                        }

                        else if (val.skillname == "senpuu")
                        {
                            val.lastuse -= (rand() % 10000 + 20000);
                        }

                        val.lastuse = nowtick + rand()%(val.cd / 20 + 1);
                        for (int i = 0; i < val.sknum; i++)
                        {
                            // if(rand()%100 >= val.rate[i]){
                            //     if(i < val.sknum - 1 && val.rate[i+1] > 0) break;
                            // }
                            FindPos(charposX, charposY, ringposX, ringposY, isOther);
                            keybd(val.KBDname[i].data(), "ALL");

                            if (rand() % 100 < 50)
                            {
                                if (val.skillname == "angryangel" || val.skillname == "javelin")
                                {
                                    if ((*charposX) > hikikae[1])
                                    {
                                        direction = 0;
                                    }
                                    else if ((*charposX) < hikikae[0])
                                    {
                                        direction = 1;
                                    }
                                    if (direction == 1)
                                    {
                                        keybd("LEFT", "UP");
                                        keybd("RIGHT", "DOWN");
                                    }
                                    else if (direction == 0)
                                    {
                                        keybd("RIGHT", "UP");
                                        keybd("LEFT", "DOWN");
                                    }
                                }
                            }

                            syscontrol(val.KBDdelay[i], timestamp, &enable, &nowtick);
                        }
                        break;
                        }
                    }
                }
                break;
            
            case 2: // solve ring
                keybd("Y", "ALL");
                syscontrol(400, timestamp, &enable, &nowtick);
                SolveWheel();
                lastring = nowtick;
                now_action = 1;
                isring = 0;
                break;
            
            case 3: // special boss
                keybd("D", "DOWN");
                while((*ifboss)){
                    FindBoss(ifboss);
                    syscontrol(500, timestamp, &enable, &nowtick);
                }
                keybd("D", "UP");
                syscontrol(40000, timestamp, &enable, &nowtick);
                now_action = 1;
                break;
            
            case 4: // move to
                if((abs((*charposX)-toX) < 5 || toX == anyway) && abs((*charposY)-toY) < 4) {
                    timecount++;
                    keybd("RIGHT", "UP");
                    keybd("LEFT", "UP");
                }
                else {
                    timecount = 0;
                    
                    if(abs(toY - (*charposY)) > 3){
                        if(toY > (*charposY) + 8){
                            keybd("DOWN", "DOWN");
                            syscontrol(100, timestamp, &enable, &nowtick);
                            keybd("C", "ALL");
                            syscontrol(100, timestamp, &enable, &nowtick);
                            keybd("C", "ALL");
                            syscontrol(100, timestamp, &enable, &nowtick);
                            keybd("DOWN", "UP");
                            syscontrol(500, timestamp, &enable, &nowtick);
                        }
                        else{
                            closestladderposX = findclosestladder((*charposX), (*charposY));
                            if(abs((*charposX) - closestladderposX) < -16){
                                if((*charposX) > closestladderposX){
                                    keybd("LEFT", "DOWN");
                                }
                                else{
                                    keybd("RIGHT", "DOWN");
                                }
                                syscontrol(100, timestamp, &enable, &nowtick);
                                keybd("C", "ALL");
                                syscontrol(360 - 20 * abs((*charposX) - closestladderposX), timestamp, &enable, &nowtick);
                                if(abs((*charposX) - closestladderposX) > 4)keybd("C", "ALL");

                                syscontrol(50, timestamp, &enable, &nowtick);
                                keybd("UP", "DOWN");
                                
                                keybd("LEFT", "UP");
                                keybd("RIGHT", "UP");

                                for (int i = 0; i < 20;i++){
                                    syscontrol(20, timestamp, &enable, &nowtick);
                                    FindPos(charposX, charposY, ringposX, ringposY, isOther);
                                    if(abs((*charposX) - findclosestladder((*charposX), (*charposY))) < 3) break;
                                }

                                if(abs((*charposX) - findclosestladder((*charposX), (*charposY))) < 3){
                                    keybd("C", "ALL");
                                    syscontrol(300, timestamp, &enable, &nowtick);
                                    keybd("UP", "UP");
                                    syscontrol(400, timestamp, &enable, &nowtick);
                                    keybd("X", "ALL");
                                    syscontrol(300, timestamp, &enable, &nowtick);
                                }
                                
                            }
                            else{
                                if((toX != anyway && (*charposX) > toX) || (toX == anyway && (*charposX) > hikikae[1])){
                                    direction = 0;
                                }
                                else if((toX != anyway && (*charposX) < toX) || (toX == anyway && (*charposX) < hikikae[0])){
                                    direction = 1;
                                }

                                if(direction == 1){
                                    keybd("LEFT", "UP");
                                    keybd("RIGHT", "DOWN");
                                }
                                else if(direction == 0){
                                    keybd("RIGHT", "UP");
                                    keybd("LEFT", "DOWN");
                                }
                                keybd("C", "ALL");
                                syscontrol(200, timestamp, &enable, &nowtick);
                                keybd("C", "ALL");
                                syscontrol(100, timestamp, &enable, &nowtick);
                                keybd("X", "ALL");
                                syscontrol(400, timestamp, &enable, &nowtick);
                                // keybd("LEFT", "UP");
                                // keybd("RIGHT", "UP");
                            }
                        }
                    }

                    FindPos(charposX, charposY, ringposX, ringposY, isOther);
                    if((toX != anyway && (*charposX) > toX) || (toX == anyway && (*charposX) > hikikae[1])){
                        direction = 0;
                    }
                    else if((toX != anyway && (*charposX) < toX) || (toX == anyway && (*charposX) < hikikae[0])){
                        direction = 1;
                    }

                    if(toX != anyway){
                        if(direction == 1){
                            keybd("LEFT", "UP");
                            keybd("RIGHT", "DOWN");
                        }
                        else if(direction == 0){
                            keybd("RIGHT", "UP");
                            keybd("LEFT", "DOWN");
                        }
                    }

                    if(abs((*charposX) - toX) > 35 && toX != anyway){
                        keybd("C", "ALL");
                        syscontrol(100, timestamp, &enable, &nowtick);
                        keybd("C", "ALL");
                        syscontrol(100, timestamp, &enable, &nowtick);
                        keybd("C", "ALL");
                        syscontrol(150, timestamp, &enable, &nowtick);
                    }
                    else{
                        syscontrol(300, timestamp, &enable, &nowtick);
                    }
                    
                    keybd("RIGHT", "UP");
                    keybd("LEFT", "UP");
                }
                if(isring && (*ringposX) == (*ringposY) && (*ringposX) == -1){
                    now_action = 2;
                    break;
                }

                if( timecount > 0){
                    timecount = 0;
                    toX = anyway;
                    if((*ifboss)) now_action = 3;
                    else if(isring) now_action = 2;
                    else if(nowtick - lastpick > pickCD) now_action = 5;
                    else now_action = 1;
                }
                break;
            
            case 5: // pick money
                lastpick = (*timestamp).tv_sec * 1000 + (*timestamp).tv_usec / 1000;

                while((*charposY) < 55){
                    keybd("DOWN", "DOWN");
                    syscontrol(50, timestamp, &enable, &nowtick);
                    keybd("C", "ALL");
                    syscontrol(50, timestamp, &enable, &nowtick);
                    keybd("C", "ALL");
                    syscontrol(50, timestamp, &enable, &nowtick);
                    keybd("DOWN", "UP");
                    syscontrol(430, timestamp, &enable, &nowtick);
                    FindPos(charposX, charposY, ringposX, ringposY, isOther);
                }
                
                if((*charposX) < 90){
                    keybd("RIGHT", "DOWN");
                }
                else{
                    keybd("LEFT", "DOWN");
                }

                while((*charposX)>30 && (*charposX) < 140){
                    keybd("C", "ALL");
                    syscontrol(160, timestamp, &enable, &nowtick);
                    keybd("C", "ALL");
                    syscontrol(250, timestamp, &enable, &nowtick);
                    keybd("X", "ALL");
                    syscontrol(680, timestamp, &enable, &nowtick);
                    FindPos(charposX, charposY, ringposX, ringposY, isOther);
                }

                keybd("RIGHT", "UP");
                keybd("LEFT", "UP");
                now_action = 1;
                break;
            
            case 6: // buff
                lastbuff = (*timestamp).tv_sec * 1000 + (*timestamp).tv_usec / 1000;
                for (int i = 0; i < skills["buff"].sknum;i++){
                    keybd(skills["buff"].KBDname[i].data(), "ALL");
                    syscontrol(skills["buff"].KBDdelay[i], timestamp, &enable, &nowtick);
                }
                now_action = 1;
                break;
            
            default:
                break;
            }
        }
    }
}

int main(){
    // struct timeval nowtime;
    int charposX, charposY, ringposX, ringposY;
    bool ifboss = 0, isOther = 0;
    set_key_value();
    set_skill();
    ScriptSystem(&nowtime, &charposX, &charposY, &ringposX, &ringposY, &ifboss, &isOther);

    return 0;
}