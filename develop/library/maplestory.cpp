#include <string>
#include <iostream>
#include <cmath>
#include <functional>
#include <opencv2/opencv.hpp>

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

void SolveWheel(SYS &script){
    std::string filename = script.gettimestring();
    PICTURE puzz(500, 150, "MapleStory");
    map<int, const char *> UDLR;
    UDLR[2] = "D";
    UDLR[4] = "L";
    UDLR[6] = "R";
    UDLR[8] = "U";
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
        filename += (string)" " + UDLR[way[i]];
    }
    cv::imwrite((string)"ringRECORD/" + filename + (string)".jpg", cv::Mat(puzz.height, puzz.width, puzz.bit == 4 ? CV_8UC4 : CV_8UC3, &puzz.Pixels[0]));
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