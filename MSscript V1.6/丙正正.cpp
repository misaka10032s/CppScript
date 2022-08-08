#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <map>
#include <windows.h>
#include <ctime>

#include <opencv2/opencv.hpp>

#include <cstdint>
#include <cstring>
#include <vector>

#include <string>
#include <sstream>

#include "imgproc.cpp"
// #include "KBDscript.cpp"
#include "skillset.cpp"
#include "sysfunc.cpp"

using namespace cv;
using namespace std;

#define WIN32_LEAN_AND_MEAN

#define 整數 int
#define 主要 main
#define 回傳 return
#define 輸出 cout
#define 換行 endl
#define 系統 system
#define 暫停 "pause"

整數 主要(){
    整數 哈哈 = 10;
    輸出 << 哈哈++ << 換行;
    系統(暫停);
    回傳 哈哈;
}
