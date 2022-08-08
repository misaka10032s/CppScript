#include <vector>
#include <iostream>
#include <windows.h>
using namespace std;

struct skilloption {
    string skillname;
    vector<string> KBDname;
    vector<int> KBDdelay;
    int sknum;
    int cd;
    long int lastuse;
};

struct skilloption spotlight;
struct skilloption ribbon;
struct skilloption blackhole;
struct skilloption buff;
struct skilloption normal;

vector<string> skillorder;
int loopskillnum = 4;

void set_skill(){
    skillorder.resize(loopskillnum);
    skillorder[0] = "spotlight";
    skillorder[1] = "ribbon";
    skillorder[2] = "blackhole";
    skillorder[3] = "normal";

    
    for(auto i : {"4"}) spotlight.KBDname.push_back(i);
    for(auto i : {1000}) spotlight.KBDdelay.push_back(i);
    spotlight.sknum = spotlight.KBDname.size();
    spotlight.cd = 200 * 1000;
    spotlight.lastuse = -1;
    spotlight.skillname = "spotlight";

    for(auto i : {"PGD"}) ribbon.KBDname.push_back(i);
    for(auto i : {1000}) ribbon.KBDdelay.push_back(i);
    ribbon.sknum = = ribbon.KBDname.size();
    ribbon.cd = 200 * 1000;
    ribbon.lastuse = -1;
    ribbon.skillname = "ribbon";

    for(auto i : {"G"}) blackhole.KBDname.push_back(i);
    for(auto i : {1000}) blackhole.KBDdelay.push_back(i);
    blackhole.sknum = = blackhole.KBDname.size();
    blackhole.cd = 200 * 1000;
    blackhole.lastuse = -1;
    blackhole.skillname = "blackhole";

    for(auto i : {"5"}) buff.KBDname.push_back(i);
    for(auto i : {10000}) buff.KBDdelay.push_back(i);
    buff.sknum = = buff.KBDname.size();
    buff.cd = 200 * 1000;
    buff.lastuse = -1;
    buff.skillname = "buff";

    for(auto i : {"C", "C", "Z"}) normal.KBDname.push_back(i);
    for(auto i : {200, 400, 400}) normal.KBDdelay.push_back(i);
    normal.sknum = = normal.KBDname.size();
    normal.cd = 200 * 1000;
    normal.lastuse = -1;
    normal.skillname = "normal";
}