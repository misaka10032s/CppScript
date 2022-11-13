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
        std::map<std::string, int> skillBind;
        std::vector<RANGE> forbidZone;

        skilloption();
        skilloption(std::string _skillname, std::initializer_list<std::string> _KBDname, std::initializer_list<int> _KBDdelay, std::initializer_list<float> _rate, int _cd);
        skilloption(std::string _skillname, std::vector<std::string> _KBDname, std::vector<int> _KBDdelay, std::vector<float> _rate, int _cd);
        // skilloption(std::string _skillname, std::vector<std::string> _KBDname, std::vector<int> _KBDdelay, std::vector<float> _rate, int _cd, std::map<std::string, int> _skillBind, std::vector<RANGE> _forbidZone);
        bool canUse(int nowtick);
};

class MSsetting{
    public:
        int now_action;
        int direction;
        pointMS to;
        int hikikae[2];
        int pickCD;
        int ringCD;
        long int lastpick;
        long int lastbuff;
        long int lastring;
        std::vector<skilloption> skills;
        std::vector<std::string> skillsOrder;
        pointMS charpos;
        pointMS ringpos;
        pointMS charStay;
        pointMS miniMapSize;
        bool isOther;
        bool isring;
        int errorpos;
        int isOthertimecount;
        int timecount;

        MSsetting();
        MSsetting(std::initializer_list<int> _hikikae, int _pickCD, pointMS _charStay, pointMS _miniMapSize, std::initializer_list<skilloption> _skills);

        void setDirection();
};

int GetEigenvalue(PICTURE &targetPic);
int GetEigenvalue2(PICTURE &targetPic);
void findarrow(PICTURE &targetPic, int way[4]);
void matchway(PICTURE &targetPic, int way[4]);
void getpos(PICTURE &targetPic, pointMS &charpos, pointMS &ringpos, bool &isOther);
void getNPC(PICTURE &targetPic, bool *isNPC);
void solveWheel(SYS &script);
void openTakenoko(SYS &script);
bool matchTakenoko(SYS &script, int type, int maxTrytime);
void getTakenoko(SYS &script);
int makeItem(SYS &script, std::vector<int> itemIndex);
pointMS getInventoryPos(int itemIndex, bool isLarge, int invPos);
void washItem(SYS &script, std::vector<int> itemIndex, bool isLarge, int invPos);
void washItemAyashii(SYS &script, std::vector<int> itemIndex, std::vector<bool> isWeapon, int invPos, int cubeIdx);
void actSkill(skilloption &skill, SYS &script, MSsetting &myMSsetting);
void actSkill(std::string skillName, SYS &script, MSsetting &myMSsetting);

int getCodename();
int readScript(int cn, MSsetting &myMSsetting);
#endif