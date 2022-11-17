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

std::map<std::string, std::vector<std::string>> scriptConfig;

inline bool isFind(std::string &key){
    return scriptConfig.find(key) == scriptConfig.end() && scriptConfig[key].size();
}

int main(){
    const char* targetWnd = "MapleStory";
    SYS scriptMS(targetWnd);
    std::string scriptType = "";

    int usedScriptId;
    if(getCodename("scripts3in1/")) {
        std::cout<<"please make scripts.\n";
        return 0;
    }
    std::cout << "input script Id.\n";
    std::cin >> usedScriptId;
    scriptConfig = readScript("scripts3in1/", usedScriptId);

    // wash, washayashii, auto make, takenoko, auto totem
    // scriptConfig.find("charStay") == scriptConfig.end()
    
    if(isFind("scriptType")) scriptType = scriptConfig["scriptType"][0];
    switch (scriptType){
        case "wash":
            std::vector<int> itemIndex;
            bool isLarge;
            int cubeIdx, invPos;
            if(!isFind("itemIndex")) break;

            // cubeIdx = stoi(scriptConfig["scriptType"][0]);
            invPos = isFind("invPos") ? stoi(scriptConfig["invPos"][0]) : 0;
            isLarge = isFind("isLarge") ? stoi(scriptConfig["isLarge"][0]) : 0;
            for(auto i: scriptConfig["itemIndex"]) itemIndex.push_back(stoi(i));
            washItem(scriptMS, itemIndex, isLarge, invPos);

            break;

        case "washayashii":
            std::vector<int> itemIndex;
            std::vector<bool> isWeapon;
            int cubeIdx, invPos;
            if(!isFind("cubeIdx") || !isFind("itemIndex")) break;

            cubeIdx = stoi(scriptConfig["scriptType"][0]);
            invPos = isFind("invPos") ? stoi(scriptConfig["invPos"][0]) : 0;
            for(auto i: scriptConfig["isWeapon"]) isWeapon.push_back(stoi(i));
            for(auto i: scriptConfig["itemIndex"]) itemIndex.push_back(stoi(i));
            washItemAyashii(scriptMS, itemIndex, isWeapon, invPos, cubeIdx);

            break;

        case "make":
            int makeTimes, cd, menuPos;
            float eatNum, eatNumCount = 0;
            std::vector<int> itemIndex;
            std::string eatKey;
            if(!isFind("makeTimes") || !isFind("cd") || !isFind("itemIndex")) break;

            makeTimes = stoi(scriptConfig["makeTimes"][0]);
            cd = stoi(scriptConfig["cd"][0]);
            menuPos = isFind("menuPos") ? stoi(scriptConfig["menuPos"][0]) : 0;
            eatNum = isFind("eatNum") ? stof(scriptConfig["eatNum"][0]) : 0;
            eatKey = isFind("eatKey") ? scriptConfig["eatKey"][0] : "INS";
            for(auto i: scriptConfig["itemIndex"]) itemIndex.push_back(stoi(i));

            for(int i=0; i<makeTimes; i++){
                scriptMS.wait(cd * 60000 - makeItem(scriptMS, itemIndex, menuPos));
                eatNumCount += eatNum;
                while(eatNumCount >= 1){
                    scriptMS.wait(300);
                    scriptMS.keybd(eatKey.c_str(), 3);
                    eatNumCount--;
                }
            }
            
            break;

        case "takenoko":
            int makeTimes, type;
            if(!isFind("makeTimes") || !isFind("type")) break;

            makeTimes = stoi(scriptConfig["makeTimes"][0]);
            type = stoi(scriptConfig["type"][0]);

            for(int i=0; i<makeTimes;){
                std::cout<<i<<"\n";
                openTakenoko(scriptMS);
                if(matchTakenoko(scriptMS, type, 100)){};
                else {
                    scriptMS.wait(30 * 60 * 1000 + 10000);
                    getTakenoko(scriptMS);
                    i++;
                }
            }

            break;

        case "totem":
            int cd;
            long int countDown;
            std::string totemKey;

            countDown = isFind("countDown") ? stoi(scriptConfig["countDown"][0]) : -1;
            cd = isFind("cd") ? stoi(scriptConfig["cd"][0]) : 5;
            totemKey = isFind("totemKey") ? scriptConfig["totemKey"][0] : "DEL";

            if(countDown > 0) scriptMS.setCountdown(countDown);
            while(1){
                scriptMS.keybd(totemKey.c_str(), 3);
                scriptMS.keybd(totemKey.c_str(), 3);
                scriptMS.wait(cd * 60 * 1000);
            }

            break;
        
        default:
            std::cout<<"unknown script name\n";
            break;
    }
    
    return 0;
}