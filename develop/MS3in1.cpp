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

inline bool isFind(const char *key){
    return scriptConfig.find((std::string)key) != scriptConfig.end() && scriptConfig[(std::string)key].size();
}

int main(){
    const char* targetWnd = "MapleStory";
    SYS scriptMS(targetWnd);
    std::string scriptType = "";

    std::vector<int> itemIndex;
    std::vector<bool> isWeapon;
    std::string eatKey, totemKey;
    bool isLarge;
    int cubeIdx, invPos, makeTimes, cd, menuPos, type, minusCD;
    long int countDown;
    float eatNum, eatNumCount = 0;

    int usedScriptId;
    while(1){
        system("cls");
        if(getCodename("scripts3in1/")) {
            std::cout<<"please make scripts.\n";
            system("pause");
            return 0;
        }

        std::cout << "input script Id.\n";
        std::cin >> usedScriptId;
        scriptConfig = readScript("scripts3in1/", usedScriptId);

        // wash, washayashii, auto make, takenoko, auto totem


        if(isFind("scriptType")) scriptType = scriptConfig["scriptType"][0];

        scriptMS.wait(200);
        if(scriptType == "wash"){
            // std::vector<int> itemIndex;
            // bool isLarge;
            // int cubeIdx, invPos;

            if(!isFind("itemIndex")){
                std::cout<<"broken script\n";
                system("pause");
                return 0;
            }

            // cubeIdx = stoi(scriptConfig["scriptType"][0]);
            invPos = isFind("invPos") ? stoi(scriptConfig["invPos"][0]) : 0;
            isLarge = isFind("isLarge") ? stoi(scriptConfig["isLarge"][0]) : 0;
            for(auto i: scriptConfig["itemIndex"]) itemIndex.push_back(stoi(i));

            
            washItem(scriptMS, itemIndex, isLarge, invPos);
        }
        else if(scriptType == "washayashii"){
            // std::vector<int> itemIndex;
            // std::vector<bool> isWeapon;
            // int cubeIdx, invPos;

            if(!isFind("cubeIdx") || !isFind("itemIndex")) {
                std::cout<<"broken script\n";
                system("pause");
                return 0;
            }

            cubeIdx = stoi(scriptConfig["cubeIdx"][0]);
            invPos = isFind("invPos") ? stoi(scriptConfig["invPos"][0]) : 0;
            for(auto i: scriptConfig["isWeapon"]) isWeapon.push_back(stoi(i));
            for(auto i: scriptConfig["itemIndex"]) itemIndex.push_back(stoi(i));

            
            washItemAyashii(scriptMS, itemIndex, isWeapon, invPos, cubeIdx);
        }
        else if(scriptType == "make"){
            // int makeTimes, cd, menuPos;
            // float eatNum, eatNumCount = 0;
            // std::vector<int> itemIndex;
            // std::string eatKey;

            if(!isFind("makeTimes") || !isFind("cd") || !isFind("itemIndex")) {
                std::cout<<"broken script\n";
                system("pause");
                return 0;
            }

            makeTimes = stoi(scriptConfig["makeTimes"][0]);
            cd = stoi(scriptConfig["cd"][0]);
            menuPos = isFind("menuPos") ? stoi(scriptConfig["menuPos"][0]) : 0;
            eatNum = isFind("eatNum") ? stof(scriptConfig["eatNum"][0]) : 0;
            eatKey = isFind("eatKey") ? scriptConfig["eatKey"][0] : "INS";
            for(auto i: scriptConfig["itemIndex"]) itemIndex.push_back(stoi(i));

            
            for(int i=0; i<makeTimes; i++){
                minusCD = makeItem(scriptMS, itemIndex, menuPos);
                scriptMS.wait(100);
                scriptMS.mouseLC(600, 50, 3); // click outside to eat
                scriptMS.wait(100);
                eatNumCount += eatNum;
                std::cout << eatNumCount << "\n";
                while(eatNumCount >= 1){
                    scriptMS.wait(500);
                    scriptMS.keybd(eatKey.c_str(), 3);
                    eatNumCount--;
                }
                scriptMS.wait(cd * 60000 - minusCD);
            }
        }
        else if(scriptType == "takenoko"){
            // int makeTimes, type;

            if(!isFind("makeTimes") || !isFind("type")) {
                std::cout<<"broken script\n";
                system("pause");
                return 0;
            }

            makeTimes = stoi(scriptConfig["makeTimes"][0]);
            type = stoi(scriptConfig["type"][0]);

            for(int i=0; i<makeTimes;){
                std::cout<<i<<"\n";
                openTakenoko(scriptMS);
                if(matchTakenoko(scriptMS, type, 100)){}
                else {
                    scriptMS.wait(30 * 60 * 1000 + 10000);
                    getTakenoko(scriptMS);
                    i++;
                }
            }
        }
        else if(scriptType == "totem"){
            // int cd;
            // long int countDown;
            // std::string totemKey;

            countDown = isFind("countDown") ? stoi(scriptConfig["countDown"][0]) : -1;
            cd = isFind("cd") ? stoi(scriptConfig["cd"][0]) : 5;
            totemKey = isFind("totemKey") ? scriptConfig["totemKey"][0] : "DEL";
            if(countDown == -1) std::cin >> countDown;

            if(countDown > 0) scriptMS.setCountdown(countDown);

            
            while(1){
                scriptMS.keybd(totemKey.c_str(), 3);
                scriptMS.keybd(totemKey.c_str(), 3);
                scriptMS.wait(cd * 60 * 1000);
            }
        }
        else {
            std::cout<<"unknown script name\n";
        }

        scriptMS.pause();
    }

    system("pause");
    return 0;
}