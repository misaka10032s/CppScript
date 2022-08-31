#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdint>
#include<direct.h>

using namespace std;

struct skilloption {
    string skillname;
    vector<string> KBDname;
    vector<int> KBDdelay;
    int delayGlobal;
    int sknum;
    int cd;
    long int lastuse;
};

struct skillbind{
    string bindSkill;
    int lastuse;
};

struct skillset {
    vector<struct skilloption> skills;
    map<string, struct skillbind> skbind;
    int pickCD;
    int stayFloor[2];
    int miniMapSize[2];
    int hikikae[2];
};

vector<string> codenameList;
struct skillset mySkillSet;

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
    string s = LTrim( srcStr , charList );
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

int readFile(string fname, string type, string* data){
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
    if (result != lSize) {
        return 3;
    }

    data -> assign(buff);
    fclose (f);
    free(buff);
    return 0;
}

int getCodename(){
    // int L;
    // string data, tmp = "";
    // readFile("scripts/codename.txt", "r", &data);
    // L = data.length();
    // for (int i = 0; i < L; i++){
    //     if(data[i] != "\n") tmp += (string)data[n];
    //     else {
    //         cout << key << "： \t" << tmp << "\n";
    //         codenameList.push_back(tmp);
    //         tmp = "";
    //     }
    // }

    int idx = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("scripts/")) != NULL) {
        while ((ent = readdir (dir)) != NULL){
            if(ent->d_name == (string)"." || ent->d_name == (string)"..") continue;
            cout << idx++ << ":\t" << ent->d_name << "\n";
            codenameList.push_back(ent->d_name);
        }
        closedir (dir);
    }
    else{
        // https://stackoverflow.com/questions/23427804/cant-find-mkdir-function-in-dirent-h-for-windows
        // https://www.796t.com/post/YTRkNXM=.html
        _mkdir("scripts/");
        cout << "make dir scripts\n";
        return 1;
    }

    return 0;
}

int initSkillSet(map<string, vector<string>> setData){
    if(setData.find("stayFloor") == setData.end()){
        mySkillSet.stayFloor[0] = 35;
        mySkillSet.stayFloor[1] = 50;
    }
    else{
        mySkillSet.stayFloor[0] = stoi(setData["stayFloor"][0]);
        mySkillSet.stayFloor[1] = stoi(setData["stayFloor"][1]);
    }

    if(setData.find("pickCD") == setData.end()){
        mySkillSet.pickCD = 1800000;
    }
    else{
        mySkillSet.pickCD = stoi(setData["pickCD"][0]);
    }

    if(setData.find("miniMapSize") == setData.end()){
        // getMiniMapSize(mySkillSet.miniMapSize);
        mySkillSet.miniMapSize[0] = 300;
        mySkillSet.miniMapSize[1] = 100;
    }
    else{
        mySkillSet.miniMapSize[0] = stoi(setData["miniMapSize"][0]);
        mySkillSet.miniMapSize[1] = stoi(setData["miniMapSize"][1]);
    }

    if(setData.find("hikikae") == setData.end()){
        mySkillSet.hikikae[0] = mySkillSet.miniMapSize[0] * 0.3;
        mySkillSet.hikikae[1] = mySkillSet.miniMapSize[0] * 0.7;
    }
    else{
        mySkillSet.hikikae[0] = stoi(setData["hikikae"][0]);
        mySkillSet.hikikae[1] = stoi(setData["hikikae"][1]);
    }

    if(setData.find("skilloption") == setData.end()){
        return 2;
    }
    else{
        // string skillname;
        // vector<string> KBDname;
        // vector<int> KBDdelay;
        // int delayGlobal;
        // int sknum;
        // int cd;
        // long int lastuse;
        // name cd delayGlobal KBDname KBDdelay lastuse
        // ex: buff_200_100_5-4-\_1000-1000-1000
        struct skilloption soTmp;
        vector<string> tmpStr, tmpStr2;
        int L;
        for(auto fl : setData["skilloption"]){
            tmpStr = split(fl, "_");
            L = tmpStr.size();
            if(L < 5) return 3;

            soTmp.skillname = tmpStr[0];
            soTmp.cd = stoi(tmpStr[1]) * 1000;
            soTmp.delayGlobal = stoi(tmpStr[2]);
            tmpStr2 = split(tmpStr[3], "-");
            for(auto kbd : tmpStr2){
                soTmp.KBDname.push_back(kbd);
            }
            tmpStr2 = split(tmpStr[4], "-");
            for(auto delay : tmpStr2){
                soTmp.KBDdelay.push_back(stoi(delay));
            }
            
            soTmp.sknum = soTmp.KBDname.size();

            if(L > 5) soTmp.lastuse = stoi(tmpStr[5]);
            else soTmp.lastuse = -1;

            mySkillSet.skills.push_back(soTmp);
        }
    }
    
    // use A skill then set B skill lastuse to specific value;
    if(setData.find("skillbind") != setData.end()){
        struct skillbind skbTmp;
        vector<string> tmpStr;
        for(auto fl : setData["skillbind"]){
            tmpStr = split(fl, "_");
            skbTmp.bindSkill = tmpStr[1];
            skbTmp.lastuse = stoi(tmpStr[2]);
            mySkillSet.skbind[tmpStr[0]] = skbTmp;
        }
        
    }

    return 0;
}

int readScript(int cn){
    string data, tmp = "";
    vector<string> lines, lineTmp;
    map<string, vector<string>> keyValue;
    if(cn < 0 || cn >= codenameList.size()) return 1;
    
    if (readFile((string) "scripts/" + codenameList[cn], "r", &data)){
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
    
    initSkillSet(keyValue);
    return 0;
}
/*
int pickCD
int stayFloor {up, down}
int miniMapSize {width, height}
int hikikae {left, right}
struct skilloption 
    skillNum
    skillName skillCd skillBtnNum
    skillBtn skillBtnDelay
*/

/*
void testFnc(){
    string a = "qwe:1\nasd:2,3,4\nzxc:5,6", s;
    vector<string> lines, lineTmp;
    map<string, vector<string>> keyValue;
    lines = split(a, "\n");
    for(auto line : lines){
        lineTmp = split(line, ":");
        keyValue[lineTmp[0]] = split(lineTmp[1], ",");
        for(auto x : lineTmp) cout<<x<<" "; cout<<"\n";
    }
    cout<<"\n";
    cin >> s;
    if(keyValue.find(s) == keyValue.end()) cout<<"NULL desu\n";
    for(auto x : keyValue[s]) cout<<x<<" "; cout<<"\n";
}
*/