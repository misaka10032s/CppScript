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

        MSsetting(std::initializer_list<int> _hikikae, int _pickCD, pointMS _charStay, pointMS _miniMapSize, std::initializer_list<skilloption> _skills);

        void setDirection();
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