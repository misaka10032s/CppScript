#include <string>
#include <iostream>
#include <cmath>
#include <functional>
#include <opencv2/opencv.hpp>
#include <direct.h>

#include "systemOS.h"
#include "maplestory.h"
#include "systemFnc.h"
#include "picture.h"
using namespace std;


pointMS::pointMS(){
    this->x = 0;
    this->y = 0;
};
pointMS::pointMS(int x, int y){
    this->x = x;
    this->y = y;
};
pointMS::pointMS(std::initializer_list<int> pos){
    if(pos.size() > 0){
        this->x = *(pos.begin());
        this->y = *(pos.begin() + 1%pos.size());
    }
    else{
        this->x = this->y = -1;
    }
};

skilloption::skilloption(){};
skilloption::skilloption(std::string _skillname, std::initializer_list<std::string> _KBDname, std::initializer_list<int> _KBDdelay, std::initializer_list<float> _rate, int _cd){
    this->skillname = _skillname;
    this->KBDname = _KBDname;
    // KBDdelay = _KBDname;
    // rate = _rate;
    this->sknum = (int)_KBDname.size();
    this->cd = _cd;
    this->lastuse = -1;

    for(int i=0; i<sknum; i++) this->KBDdelay.push_back(*(_KBDdelay.begin() + i%((int)_KBDdelay.size())));
    for(int i=0; i<sknum; i++) this->rate.push_back(*(_rate.begin() + i%((int)_rate.size())));
};
skilloption::skilloption(std::string _skillname, std::vector<std::string> _KBDname, std::vector<int> _KBDdelay, std::vector<float> _rate, int _cd){
    this->skillname = _skillname;
    this->KBDname = _KBDname;
    // KBDdelay = _KBDname;
    // rate = _rate;
    this->sknum = (int)_KBDname.size();
    this->cd = _cd;
    this->lastuse = -1;

    for(int i=0; i<sknum; i++) this->KBDdelay.push_back(_KBDdelay[i%((int)_KBDdelay.size())]);
    for(int i=0; i<sknum; i++) this->rate.push_back(_rate[i%((int)_rate.size())]);
};

MSsetting::MSsetting(std::initializer_list<int> _hikikae, int _pickCD, pointMS _charStay, pointMS _miniMapSize, std::initializer_list<skilloption> _skills){
    this->now_action = 1;
    this->direction = -1;
    this->to = {-1, -1};
    this->hikikae[0] = *(_hikikae.begin());
    this->hikikae[1] = *(_hikikae.begin() + 1);
    this->pickCD = _pickCD;
    this->ringCD = 15 * 60 * 1000 + 5000;
    this->lastpick = -1;
    this->lastbuff = -1;
    this->lastring = -1;
    this->skills = _skills;
    for(int i=0; i<(int)this->skills.size(); i++)this->skillsOrder.push_back(this->skills[i].skillname);
    this->charpos = {-1, -1};
    this->ringpos = {-1, -1};
    this->charStay = _charStay;
    this->miniMapSize = _miniMapSize;
    this->isOther = 0;
    this->isring = 0;
    this->errorpos = 0;
    this->isOthertimecount = 0;
    this->timecount = 0;
}

void MSsetting::setDirection(){
    if(this->direction == -1) this->direction = this->charpos.x > this->miniMapSize.x/2 ? 0 : 1;
    else if (this->charpos.x > this->hikikae[1]) this->direction = 0;
    else if (this->charpos.x < this->hikikae[0]) this->direction = 1;
};


int GetEigenvalue(PICTURE &targetPic){
    int e = 0;
    for (int j=0; j<targetPic.height; j++) {
        for(int i=0; i<targetPic.width; i++){
            for(int k=0; k<3; k++){
                e += targetPic.Pixels[i * targetPic.bit + j * targetPic.width * targetPic.bit + k];
            }
        }
    }
    return e;
}

int GetEigenvalue2(PICTURE &targetPic){
    int e = 0;
    for (int j=0; j<targetPic.height; j++) {
        for(int i=0; i<targetPic.width; i++){
            for(int k=0; k<3; k++){
                e += (targetPic.Pixels[i * targetPic.bit + j * targetPic.width * targetPic.bit + k] / 16) * 16;
            }
        }
    }
    return e;
}

void findarrow(PICTURE &targetPic, int way[4]){
    int idx=0, dy, dx, mdx, mdy, wi = 0;
    float pointNum, judgeL, judgeR, judgeT, judgeB, symX, symY;
    bool isreced, rangeup;
    PICTURE arrowPic;
    std::vector<struct RANGE> dots;
    std::vector<int> dotsX, dotsY;

    arrowPic.resize(targetPic.width, targetPic.height, targetPic.bit);
    for(int y=0; y<targetPic.height; y++){
        for(int x=0; x<targetPic.width; x++){
            for(int k=0; k<3; k++) arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + k] = 255;
        }
    }

    for(int y=8;y<targetPic.height-8;y++){
        for(int x=8;x<targetPic.width-8;x++){
            if(targetPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] == 0){
                pointNum = 0;
                for(int j=0;j<=16;j++){
                    for(int i=0;i<=16;i++){
                        if(targetPic.Pixels[(x+i-8)*targetPic.bit + (y+j-8)*targetPic.width*targetPic.bit] == 0) {
                            pointNum += 16-abs(i-8)-abs(j-8);
                        }
                    }
                }
                
                if(pointNum > 1900){
                    for(int j=0;j<=16;j++){
                        for(int i=0;i<=16;i++){
                            for(int k=0;k<3;k++)arrowPic.Pixels[(x+i-8)*targetPic.bit + (y+j-8)*targetPic.width*targetPic.bit + k] = targetPic.Pixels[(x+i-8)*targetPic.bit + (y+j-8)*targetPic.width*targetPic.bit + k];
                        }
                    }
                }
            }
        }
    }
    
    for (int x = 1; x < targetPic.width - 1; x++){
        for(int y=1;y<targetPic.height-1;y++){
            if(arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] != 0){
                if(arrowPic.Pixels[(x+1)*targetPic.bit + y*targetPic.width*targetPic.bit] == 0 && arrowPic.Pixels[(x-1)*targetPic.bit + y*targetPic.width*targetPic.bit] == 0) arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] = 0;
                if(arrowPic.Pixels[(x+1)*targetPic.bit + (y+1)*targetPic.width*targetPic.bit] == 0 && arrowPic.Pixels[(x-1)*targetPic.bit + (y-1)*targetPic.width*targetPic.bit] == 0) arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] = 0;
                if(arrowPic.Pixels[x*targetPic.bit + (y+1)*targetPic.width*targetPic.bit] == 0 && arrowPic.Pixels[x*targetPic.bit + (y-1)*targetPic.width*targetPic.bit] == 0) arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] = 0;
                if(arrowPic.Pixels[(x+1)*targetPic.bit + (y-1)*targetPic.width*targetPic.bit] == 0 && arrowPic.Pixels[(x-1)*targetPic.bit + (y+1)*targetPic.width*targetPic.bit] == 0) arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] = 0;
            }
        }
    }

    for(int x=1;x<targetPic.width-1;x++){
        for(int y=1;y<targetPic.height-1;y++){
            if(arrowPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] == 0){
                isreced = 0;
                for(int i=0;i<idx;i++){
                    isreced |= (x >= dots[i].left) && (x <= dots[i].right) && (y >= dots[i].top) && (y <= dots[i].bottom);
                }
                if (isreced) continue;
                mdx = mdy = dx = dy = 0;
                while(arrowPic.Pixels[(x+dx)*targetPic.bit + y*targetPic.width*targetPic.bit] == 0 && x+dx<targetPic.width) dx++;
                while(arrowPic.Pixels[x*targetPic.bit + (y+dy)*targetPic.width*targetPic.bit] == 0 && y+dy<targetPic.height) dy++;
                
                for(int j=1;;j++){
                    rangeup = 0;
                    for(int i=0;i<dx;i++){
                        rangeup |= arrowPic.Pixels[(x+i)*targetPic.bit + (y-j)*targetPic.width*targetPic.bit] == 0;
                    }
                    if(rangeup && y-mdy>1)mdy++;
                    else break;
                }
                for(int j=1;;j++){
                    rangeup = 0;
                    for(int i=0;i<dx;i++){
                        rangeup |= arrowPic.Pixels[(x+i)*targetPic.bit + (y+dy)*targetPic.width*targetPic.bit] == 0;
                    }
                    if(rangeup && y+dy<targetPic.height-1)dy++;
                    else break;
                }
                for(int i=1;;i++){
                    rangeup = 0;
                    for(int j=0;j<dy+mdy;j++){
                        rangeup |= arrowPic.Pixels[(x+dx)*targetPic.bit + (y-mdy+j)*targetPic.width*targetPic.bit] == 0;
                    }
                    if(rangeup && x+dx<targetPic.width-1)dx++;
                    else break;
                }
                dots.resize(idx + 1);
                dots[idx].left = x-mdx;
                dots[idx].right = x+dx-1;
                dots[idx].top = MAX(y-mdy, 0);
                dots[idx].bottom = y+dy-1;
                idx++;
                for(int i=dots[idx-1].left;i<=dots[idx-1].right;i++) {
                    arrowPic.Pixels[i*targetPic.bit + dots[idx-1].top*targetPic.width*targetPic.bit+1] = 0;
                    arrowPic.Pixels[i*targetPic.bit + dots[idx-1].bottom*targetPic.width*targetPic.bit+1] = 0;
                }
                for(int i=dots[idx-1].top;i<=dots[idx-1].bottom;i++) {
                    arrowPic.Pixels[dots[idx-1].left*targetPic.bit + i*targetPic.width*targetPic.bit+1] = 0;
                    arrowPic.Pixels[dots[idx-1].right*targetPic.bit + i*targetPic.width*targetPic.bit+1] = 0;
                }
                
            }
        }
    }
    
    for(int i=0;i<idx;i++){
        symX = symY = 0;
        if(dots[i].right == targetPic.width-1 || dots[i].left == 0 || dots[i].bottom == targetPic.height-1 || dots[i].top == 0)continue;
        if(dots[i].right - dots[i].left > 30 || dots[i].bottom - dots[i].top > 30) continue;
        dotsX.resize(dots[i].right-dots[i].left+1);
        dotsY.resize(dots[i].bottom-dots[i].top+1);

        for(int y=dots[i].top;y<=dots[i].bottom;y++){
            dotsY[y-dots[i].top] = 0;
            for(int x=dots[i].left;x<=dots[i].right;x++){
                if(arrowPic.Pixels[x*targetPic.bit+y*targetPic.width*targetPic.bit] == 0){
                    dotsY[y-dots[i].top]++;
                }
            }
        }

        for(int x=dots[i].left;x<=dots[i].right;x++){
            dotsX[x-dots[i].left] = 0;
            for(int y=dots[i].top;y<=dots[i].bottom;y++){
                if(arrowPic.Pixels[x*targetPic.bit+y*targetPic.width*targetPic.bit] == 0){
                    dotsX[x-dots[i].left]++;
                }
            }
        }

        for(int x=0;x<=dots[i].right-dots[i].left;x++){
            symX += abs(dotsX[x] - dotsX[dots[i].right-dots[i].left-x]);
        }

        for(int y=0;y<=dots[i].bottom-dots[i].top;y++){
            symY += abs(dotsY[y] - dotsY[dots[i].bottom-dots[i].top-y]);
        }

        judgeL = dotsX[3] - dotsX[1];
        judgeR = dotsX[dots[i].right-dots[i].left-3] - dotsX[dots[i].right-dots[i].left-1];
        judgeT = dotsY[3] - dotsY[1];
        judgeB = dotsY[dots[i].bottom-dots[i].top-3] - dotsY[dots[i].bottom-dots[i].top-1];

        if(wi > 3) break;
        if(symY > symX){
            if(judgeT < judgeB) way[wi++] = 2;
            else way[wi++] = 8;
        }
        else{
            if(judgeL < judgeR) way[wi++] = 6;
            else way[wi++] = 4;
        }
    }
    // imshow("Display orig", Mat(targetPic.height, targetPic.width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &arrowPic.Pixels[0]));
}

void matchway(PICTURE &targetPic, int way[4]){
    PICTURE blurPic;
    bool want;

    targetPic.GaussBlur(blurPic, 1);
    for(int y=1;y<targetPic.height-1;y++){
        for(int x=1;x<targetPic.width-1;x++){
            want = (blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] < 100) && ((blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] > 200) || (blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] > 120)) && (blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1]+blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] > 260);

            if(want){
                blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] = 0;
                blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] = 0;
                blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] = 0;
            }
            else {
                blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] = 255;
                blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] = 255;
                blurPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] = 255;
            }
        }
    }
    
    findarrow(blurPic, way);
}

void getpos(PICTURE &targetPic, pointMS &charpos, pointMS &ringpos, bool &isOther){
    PICTURE filterPic, matchPic;
    int pts[3], pointNum;
    struct RANGE range[3];
    std::vector<pointMS> pos[3];
    bool isout, checkpoint;
    // type 0 char, 1 other, 2 ring
    pts[0] = pts[1] = pts[2] = 0;
    
    for (int type = 0; type < 3;type++){
        range[type].left = range[type].right = range[type].top = range[type].bottom = -1;
    }
    // cout << BitsPerPixel << "\n";
    targetPic.screenShot(5, 18);
    matchPic.resize(targetPic.width, targetPic.height, targetPic.bit);
    targetPic.GaussBlur(filterPic, 1);
    // imshow("Display blur", Mat(targetPic.height, targetPic.width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &filterPic.Pixels[0]));
    // waitKey(0);
    for(int y=0;y<targetPic.height;y++){
        for(int x=0;x<targetPic.width;x++){
            for(int type=0;type<3;type++){
                checkpoint = (type == 0 && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] + filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] > 230) && (abs(filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] - filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2]) < 25) && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit] < 90)) ||
                (type == 1 && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0] + filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] < 200) && (abs(filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] - filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0]) < 30) && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] > 150)) ||
                (type == 2 && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0] + filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] > 200) && (abs(filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] - filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0]) < 30) && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] < 100));

                if(checkpoint){
                    pos[type].resize(pts[type]+1);
                    pos[type][pts[type]].x = x;
                    pos[type][pts[type]].y = y;
                    pts[type]++;
                    matchPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + type] = filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + type];
                }
                else {
                    matchPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + type] = 0;
                }
            }
        }
    }
    
    // cout<<"pts:"<<pts[0]<<" "<<pts[1]<<" "<<pts[2]<<"\n";
    if(!(pts[0]+pts[1]+pts[2])) {
        cout << "@@@\n";
        charpos = {-1};
        ringpos = {-1};
        isOther = 0;
        return;
    }

    for(int type=0;type<3;type++){
        for(int idx=0;idx<pts[type];idx++){
            if(matchPic.Pixels[pos[type][idx].x*targetPic.bit + pos[type][idx].y*targetPic.width*targetPic.bit + type] != 0){
                pointNum = 0;
                for(int j=0;j<=10;j++){
                    for(int i=0;i<=10;i++){
                        if(matchPic.Pixels[(pos[type][idx].x+i-5)*targetPic.bit + (pos[type][idx].y+j-5)*targetPic.width*targetPic.bit+ type] != 0) {
                            pointNum += 10-abs(i-5)-abs(j-5);
                        }
                    }
                }
                isout = (pos[type][idx].x < 5) || (pos[type][idx].x > targetPic.width - 5) || (pos[type][idx].y < 5) || (pos[type][idx].y > targetPic.height - 5);
                isout = (type == 0 && (pointNum < 90/*180*/ || isout)) || (type == 1 && (pointNum < 80 || isout)) || (type == 2 && (pointNum < 160 || isout));
                if(isout){
                    for(int j=idx;j<pts[type]-1;j++){
                        pos[type][j].x = pos[type][j+1].x;
                        pos[type][j].y = pos[type][j+1].y;
                    }
                    pts[type]--;
                    idx--;
                }
            }
        }
    }

    for(int type=0;type<3;type++){
        if(!pts[type])continue;

        range[type].left = range[type].right = pos[type][0].x;
        range[type].top = range[type].bottom = pos[type][0].y;

        for(int i=0;i<pts[type];i++){
            if(pos[type][i].x > range[type].right) range[type].right = pos[type][i].x;
            else if(pos[type][i].x < range[type].left) range[type].left = pos[type][i].x;
            if(pos[type][i].y > range[type].bottom) range[type].bottom = pos[type][i].y;
            else if(pos[type][i].y < range[type].top) range[type].top = pos[type][i].y;
        }
        // cout << range[0].left << " " << range[0].right << " " << range[0].top << " " << range[0].bottom << "\n";
        charpos.x = (range[0].left + range[0].right) / 2;
        charpos.y = (range[0].top + range[0].bottom) / 2;
        
        ringpos.x = (range[2].left + range[2].right) / 2;
        ringpos.y = (range[2].top + range[2].bottom) / 2;

        isOther = range[1].top == -1 ? 0 : 1;

        for(int i=range[type].left;i<=range[type].right;i++) {
            targetPic.Pixels[i*targetPic.bit + range[type].top*targetPic.width*targetPic.bit+1] = 255;
            targetPic.Pixels[i*targetPic.bit + range[type].bottom*targetPic.width*targetPic.bit+1] = 255;
        }
        for(int i=range[type].top;i<=range[type].bottom;i++) {
            targetPic.Pixels[range[type].left*targetPic.bit + i*targetPic.width*targetPic.bit+1] = 255;
            targetPic.Pixels[range[type].right*targetPic.bit + i*targetPic.width*targetPic.bit+1] = 255;
        }
    }
    // imshow("Display pos", Mat(targetPic.height, targetPic.width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &matchPic.Pixels[0]));
    // imshow("Display selpos", Mat(targetPic.height, targetPic.width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &targetPic.Pixels[0]));
    // waitKey(0);
}

void getNPC(PICTURE &targetPic, bool *isNPC){
    PICTURE filterPic, matchPic;
    int pts[3], pointNum;
    struct RANGE range[3];
    std::vector<pointMS> pos[3];
    bool isout, checkpoint;
    // type 0 NPC, 1 -, 2 -
    pts[0] = pts[1] = pts[2] = 0;
    
    for (int type = 0; type < 3;type++){
        range[type].left = range[type].right = range[type].top = range[type].bottom = -1;
    }

    matchPic.resize(targetPic.width, targetPic.height, targetPic.bit);
    targetPic.GaussBlur(filterPic, 1);
    // imshow("Display blur", Mat(targetPic.height, targetPic.width, targetPic.bit > 24 ? CV_8UC4 : CV_8UC3, &filterPic.Pixels[0]));
    // waitKey(0);
    for(int y=0;y<targetPic.height;y++){
        for(int x=0;x<targetPic.width;x++){
            for(int type=0;type<1;type++){
                checkpoint = (type == 0 && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0] + filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] < 100) && (abs(filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0] - filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2]) < 25) && (filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] > 200));

                if(checkpoint){
                    pos[type].resize(pts[type]+1);
                    pos[type][pts[type]].x = x;
                    pos[type][pts[type]].y = y;
                    pts[type]++;
                    matchPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + type] = filterPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + type];
                }
                else {
                    matchPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + type] = 0;
                }
            }
        }
    }
    
    // cout<<"pts:"<<pts[0]<<" "<<pts[1]<<" "<<pts[2]<<"\n";
    if(!(pts[0]+pts[1]+pts[2])) {
        cout << "@@@\n";
        (*isNPC) = 0;
        return;
    }

    for(int type=0;type<1;type++){
        for(int idx=0;idx<pts[type];idx++){
            if(matchPic.Pixels[pos[type][idx].x*targetPic.bit + pos[type][idx].y*targetPic.width*targetPic.bit + type] != 0){
                pointNum = 0;
                for(int j=0;j<=10;j++){
                    for(int i=0;i<=10;i++){
                        if(matchPic.Pixels[(pos[type][idx].x+i-5)*targetPic.bit + (pos[type][idx].y+j-5)*targetPic.width*targetPic.bit+ type] != 0) {
                            pointNum += 10-abs(i-5)-abs(j-5);
                        }
                    }
                }
                isout = (pos[type][idx].x < 5) || (pos[type][idx].x > targetPic.width - 5) || (pos[type][idx].y < 5) || (pos[type][idx].y > targetPic.height - 5);
                isout = (type == 0 && (pointNum < 60 || isout)) || (type == 1 && (pointNum < 80 || isout)) || (type == 2 && (pointNum < 160 || isout));
                if(isout){
                    for(int j=idx;j<pts[type]-1;j++){
                        pos[type][j].x = pos[type][j+1].x;
                        pos[type][j].y = pos[type][j+1].y;
                    }
                    pts[type]--;
                    idx--;
                }
            }
        }
    }

    for(int type=0;type<3;type++){
        if(!pts[type])continue;

        range[type].left = range[type].right = pos[type][0].x;
        range[type].top = range[type].bottom = pos[type][0].y;

        for(int i=0;i<pts[type];i++){
            if(pos[type][i].x > range[type].right) range[type].right = pos[type][i].x;
            else if(pos[type][i].x < range[type].left) range[type].left = pos[type][i].x;
            if(pos[type][i].y > range[type].bottom) range[type].bottom = pos[type][i].y;
            else if(pos[type][i].y < range[type].top) range[type].top = pos[type][i].y;
        }
        cout << range[0].left << " " << range[0].right << " " << range[0].top << " " << range[0].bottom << "\n";

        (*isNPC) = range[0].top == -1 ? 0 : 1;

        for(int i=range[type].left;i<=range[type].right;i++) {
            targetPic.Pixels[i*targetPic.bit + range[type].top*targetPic.width*targetPic.bit+1] = 255;
            targetPic.Pixels[i*targetPic.bit + range[type].bottom*targetPic.width*targetPic.bit+1] = 255;
        }
        for(int i=range[type].top;i<=range[type].bottom;i++) {
            targetPic.Pixels[range[type].left*targetPic.bit + i*targetPic.width*targetPic.bit+1] = 255;
            targetPic.Pixels[range[type].right*targetPic.bit + i*targetPic.width*targetPic.bit+1] = 255;
        }
    }
    // imshow("Display blur", Mat(targetPic.height, targetPic.width, targetPic.bit > 24 ? CV_8UC4 : CV_8UC3, &targetPic.Pixels[0]));
    // waitKey(0);
}

void solveWheel(SYS &script){
    std::string filename = script.gettimestring();
    PICTURE puzz(500, 150, "MapleStory");
    map<int, const char *> UDLR;
    UDLR[2] = "DOWN";
    UDLR[4] = "LEFT";
    UDLR[6] = "RIGHT";
    UDLR[8] = "UP";
    int x = 450, y = 130, way[4];
    for (int i = 0; i < 4;i++) way[i] = 0;
    puzz.screenShot(x, y);
    matchway(puzz, way);
    if(way[3] == 0) return;
    Sleep(100);
    for (int i = 0; i < 4;i++){
        script.keybd(UDLR[way[i]], 3);
        Sleep(200);
    }
    Sleep(30);
    for (int i = 0; i < 4;i++) {
        filename += (std::string)" " + UDLR[way[i]][0];
    }
    cv::imwrite((std::string)"ringRECORD/" + filename + (std::string)".jpg", cv::Mat(puzz.height, puzz.width, puzz.bit == 4 ? CV_8UC4 : CV_8UC3, &puzz.Pixels[0]));
}

int herb[15][30][3] = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
int mineral[15][30][3] = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 255}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
int takenoko(PICTURE &targetPic, int type){
    int diff = 0;
    int ref[15][30][3];
    switch(type){
        case 0:
            std::copy(&herb[0][0][0], &herb[14][29][2], &ref[0][0][0]);
            break;
        case 1:
            std::copy(&mineral[0][0][0], &mineral[14][29][2], &ref[0][0][0]);
            break;
        default:
            std::copy(&herb[0][0][0], &herb[14][29][2], &ref[0][0][0]);
            break;
    }
    std::vector<uint8_t> koPixels;
    koPixels.resize(targetPic.width * targetPic.height * targetPic.bit);

    for(int y=0;y<targetPic.height;y++){
        for(int x=0;x<targetPic.width;x++){
            for(int k=0;k<3;k++){
                koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + k] = 15 * (targetPic.Pixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + k]/15);
            }

            if(koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] >= 165 && abs(koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0] - koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1]) <= 45 && abs(koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] - koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1]) >= 60){
                koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 0] = 0;
                koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 1] = 0;
                koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + 2] = 255;
            }
            else{
                for(int k=0;k<3;k++) koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + k] = 0;
            }

            for(int k=0;k<3;k++){
                if(koPixels[x*targetPic.bit + y*targetPic.width*targetPic.bit + k] != ref[y][x][k]) diff++;
            }
        }
    }
    return diff;
}

void actSkill(skilloption &skill, SYS &script, MSsetting &myMSinfo){
    // for(int i=0; i<(int)skill.forbidZone.size(); i++) if(skill.forbidZone[i].left<=myMSinfo.charpos.x && myMSinfo.charpos.x<=skill.forbidZone[i].right && skill.forbidZone[i].top<=myMSinfo.charpos.y && myMSinfo.charpos.y<=skill.forbidZone[i].bottom) return;

    int nowtick = script.getNowtick(), skIndex;

    for (int i=0; i<skill.sknum; i++){
        script.keybd(skill.KBDname[i].data(), 3);
        script.wait(skill.KBDdelay[i]);
    }

    skill.lastuse = nowtick + rand()%(skill.cd / 20 + 1);

    // for(auto [sk, val] : skill.skillBind){
    //     skIndex = myMSinfo.skillsOrder.find(sk);
    //     if(skIndex != -1){
    //         myMSinfo.skills[skIndex].lastuse = MAX(nowtick - val, myMSinfo.skills[skIndex].lastuse);
    //     }
    // }
};
















std::vector<std::string> codenameList;

bool dirExists(const std::string& dirName_in){
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!

    return false;    // this is not a directory!
}
const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}
std::string LTrim( std::string& srcStr , const std::string& charList){
    std::string tagStr = srcStr;
    tagStr.erase( 0 , tagStr.find_first_not_of( charList.c_str() ) );
    return tagStr;
}
std::string RTrim( std::string& srcStr , const std::string& charList){
    std::string tagStr = srcStr;
    tagStr.erase( tagStr.find_last_not_of( charList.c_str() ) + 1 );
    return tagStr;
}
std::string Trim( std::string& srcStr , const std::string& charList ){
    std::string s = LTrim( srcStr , charList );
    return RTrim( s , charList );
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int readFile(std::string fname, std::string type, std::string* data){
    FILE *f;
    char* buff;
    long lSize;
    size_t result;
    
    f = fopen(fname.data(), type.data());
    if(f == NULL){
        return 1;
    }

    fseek (f , 0 , SEEK_END);
    lSize = ftell(f);
    rewind (f);

    buff = (char*) malloc (sizeof(char)*lSize);
    if (buff == NULL) {
        return 2;
    }

    result = fread (buff, 1, lSize, f);
    if ((long)result != lSize) {
        return 3;
    }

    data -> assign(buff);
    fclose (f);
    free(buff);
    return 0;
}

int getCodename(){
    int idx = 0;
    std::string path = "scripts/";
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile("scripts/", &data);
    if(hFind != INVALID_HANDLE_VALUE){
        do{
            if((std::string)data.cFileName == "." || (std::string)data.cFileName == "..") continue;
            std::cout << idx++ << ":\t" << data.cFileName << "\n";
            codenameList.push_back(data.cFileName);
        }
        while(FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    else{
        // https://stackoverflow.com/questions/23427804/cant-find-mkdir-function-in-dirent-h-for-windows
        // https://www.796t.com/post/YTRkNXM=.html
        // https://stackoverflow.com/questions/41404711/how-to-list-files-in-a-directory-using-the-windows-api
        _mkdir("scripts/");
        cout << "make dir scripts\n";
        return 1;
    }

    return 0;
}

int initSkillSet(std::map<std::string, std::vector<std::string>> &setData, MSsetting &myMSsetting){
    if(setData.find("charStay") == setData.end()){
        myMSsetting.charStay = {35, 50};
    }
    else{
        myMSsetting.charStay = {stoi(setData["stayFloor"][0]), stoi(setData["stayFloor"][1])};
    }

    if(setData.find("pickCD") == setData.end()){
        myMSsetting.pickCD = 1800000;
    }
    else{
        myMSsetting.pickCD = stoi(setData["pickCD"][0]);
    }

    if(setData.find("miniMapSize") == setData.end()){
        // getMiniMapSize(mySkillSet.miniMapSize);
        myMSsetting.miniMapSize = {300, 100};
    }
    else{
        myMSsetting.miniMapSize = {stoi(setData["miniMapSize"][0]), stoi(setData["miniMapSize"][1])};
    }

    if(setData.find("hikikae") == setData.end()){
        myMSsetting.hikikae[0] = myMSsetting.miniMapSize.x * 0.3;
        myMSsetting.hikikae[1] = myMSsetting.miniMapSize.y * 0.7;
    }
    else{
        myMSsetting.hikikae[0] = stoi(setData["hikikae"][0]);
        myMSsetting.hikikae[1] = stoi(setData["hikikae"][1]);
    }

    if(setData.find("skilloption") == setData.end()){
        return 2;
    }
    else{
        // name cd KBDname KBDdelay rate
        // ex: buff_200_100_5-4-\_1000-1000-1000_100-100-100
        skilloption soTmp;
        std::vector<std::string> tmpStr, tmpStr2;
        std::vector<int> _KBDdelay;
        std::vector<float> _rate;
        int L;
        for(auto fl : setData["skilloption"]){
            tmpStr = split(fl, "_");
            L = tmpStr.size();
            if(L < 5) return 3;
            tmpStr2 = split(tmpStr[3], "-");
            for(int i=0; i<(int)tmpStr2.size(); i++) _KBDdelay.push_back(stoi(tmpStr2[i]));
            tmpStr2 = split(tmpStr[4], "-");
            for(int i=0; i<(int)tmpStr2.size(); i++) _rate.push_back(std::stof(tmpStr2[i]));
            soTmp = skilloption(tmpStr[0], split(tmpStr[2], "-"), _KBDdelay, _rate, stoi(tmpStr[1]) * 1000);

            myMSsetting.skills.push_back(soTmp);
            myMSsetting.skillsOrder.push_back(tmpStr[0]);
        }
    }
    
    // @@@@
    // use A skill then set B skill lastuse to specific value;
    if(setData.find("skillbind") != setData.end()){
        std::vector<std::string> tmpStr;
        std::vector<std::string>::iterator skIdx;
        for(auto fl : setData["skillbind"]){
            tmpStr = split(fl, "_");
            skIdx = std::find(myMSsetting.skillsOrder.begin(), myMSsetting.skillsOrder.end(), tmpStr[0]);
            if(skIdx != myMSsetting.skillsOrder.end()){
                myMSsetting.skills[skIdx - myMSsetting.skillsOrder.begin()].skillBind.emplace(tmpStr[1], stoi(tmpStr[2]));
            }
        }
    }

    return 0;
}

int readScript(int cn, MSsetting &myMSsetting){
    std::string data, tmp = "";
    std::vector<std::string> lines, lineTmp;
    std::map<std::string, std::vector<std::string>> keyValue;
    if(cn < 0 || cn >= (int)codenameList.size()) return 1;
    
    if (readFile((std::string) "scripts/" + codenameList[cn], "r", &data)){
        cout << codenameList[cn] << " not found.\n";
        return 1;
    }
    replaceAll(data, " ", "");
    replaceAll(data, "\n", "");
    replaceAll(data, "\t", "");
    lines = split(data, ";");
    for(auto line : lines){
        lineTmp = split(line, ":");
        keyValue[lineTmp[0]] = split(lineTmp[1], ",");
    }
    
    initSkillSet(keyValue, myMSsetting);
    return 0;
}