#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "picture.h"
#include "systemOS.h"
#include "systemFnc.h"

#ifndef LIB_MAPLESTORY_H_
#define LIB_MAPLESTORY_H_

class pointMS{
    public:
        int x;
        int y;

        pointMS();
        pointMS(int x, int y);
        pointMS(std::initializer_list<int> pos);
};

class skilloption {
    public:
        std::string skillname;
        std::vector<std::string> KBDname;
        std::vector<int> KBDdelay;
        std::vector<float> rate;
        int sknum;
        int cd;
        long int lastuse;

        skilloption();
        skilloption(std::string sn, std::initializer_list<std::string> Kn, std::initializer_list<int> Kd, std::initializer_list<float> rt, int c);
};

int GetEigenvalue(PICTURE &targetPic);
int GetEigenvalue2(PICTURE &targetPic);
void findarrow(PICTURE &targetPic, int way[4]);
void matchway(PICTURE &targetPic, int way[4]);
void getpos(PICTURE &targetPic, pointMS &charpos, pointMS &ringpos, bool &isOther);
void getNPC(PICTURE &targetPic, bool *isNPC);
int takenoko(PICTURE &targetPic, int ref[15][30][3]);
void solveWheel(SYS &script);
void actSkill(skilloption &skill, SYS &script);
#endif