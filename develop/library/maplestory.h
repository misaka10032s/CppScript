#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "systemOS.h"

#ifndef LIB_MAPLESTORY_H_
#define LIB_MAPLESTORY_H_


int GetEigenvalue(PICTURE &targetPic);
int GetEigenvalue2(PICTURE &targetPic);
void findarrow(PICTURE &targetPic, int way[4]);
void matchway(PICTURE &targetPic, int way[4]);
void getpos(PICTURE &targetPic, int charpos[2], int ringpos[2], bool *isOther);
void getNPC(PICTURE &targetPic, bool *isNPC);
int takenoko(PICTURE &targetPic, int ref[15][30][3]);

#endif