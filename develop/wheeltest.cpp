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

int main(){
    PICTURE datafile;
    int way[4];
    std::string fileName;
    
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile("ringRECORD/*.*", &data);
    if(hFind != INVALID_HANDLE_VALUE){
        do{
            if((std::string)data.cFileName == "." || (std::string)data.cFileName == "..") continue;
            std::cout << data.cFileName << "\n";
            fileName = "ringRECORD/" + (std::string)data.cFileName;
            if(datafile.open(fileName)){
                matchway(datafile, way).display(fileName);
                std::cout << way[0] << ", " << way[1] << ", " << way[2] << ", " << way[3] << "\n";
            };
        }
        while(FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    
    system("pause");
    return 0;
}