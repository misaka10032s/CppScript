#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "picture.h"
#include "systemOS.h"

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

int GetEigenvalue(PICTURE &targetPic);
int GetEigenvalue2(PICTURE &targetPic);
void findarrow(PICTURE &targetPic, int way[4]);
void matchway(PICTURE &targetPic, int way[4]);
void getpos(PICTURE &targetPic, pointMS &charpos, pointMS &ringpos, bool &isOther);
void getNPC(PICTURE &targetPic, bool *isNPC);
int takenoko(PICTURE &targetPic, int ref[15][30][3]);

#endif