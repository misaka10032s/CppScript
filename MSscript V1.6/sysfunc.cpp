#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "KBDscript.cpp"

using namespace std;
struct timeval nowtime;
long int nowtick;
bool check_WHD = 0;

int gettimeofday(struct timeval * tp, struct timezone * tzp){
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

string gettimestring(){
    struct timeval tp;
    char s[12];
    gettimeofday(&tp, NULL);
    long int num = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    itoa(num, s, 10);
    return (string)s;
}

void syscontrol(int dur, struct timeval *timestamp, bool *enable, long int *nowtick){
    gettimeofday(timestamp, NULL);
    HWND MS = FindWindow(NULL, (LPCSTR)"MapleStory");
    long int st = (*timestamp).tv_sec * 1000 + (*timestamp).tv_usec / 1000, ed;
    bool frontIsMS, numLockOn;
    while(1){
        gettimeofday(timestamp, NULL);
        frontIsMS = GetForegroundWindow() == MS;
        numLockOn = LOWORD(GetKeyState(VK_NUMLOCK));
        ed = (*timestamp).tv_sec * 1000 + (*timestamp).tv_usec / 1000;
        if(ed - st > dur && (*enable) && (frontIsMS || check_WHD)){
            if(ed - st > dur + 1000) Sleep(100);
            break;
        }
        if (GetAsyncKeyState(VK_F10) < 0) {
            *enable ^= 1; 
            if(((*enable) == numLockOn && frontIsMS)){
                keybd("NUMLOCK", "ALL");
                Sleep(50);
            }
            cout << ((*enable) ? "work": "stop")  << "\n";
            Sleep(200);
        }

        if((!frontIsMS && !numLockOn) || ((frontIsMS) && (*enable) && numLockOn)){
            keybd("NUMLOCK", "ALL");
            Sleep(50);
        }

        Sleep(10);
    }
    gettimeofday(timestamp, NULL);
    *nowtick = (*timestamp).tv_sec * 1000 + (*timestamp).tv_usec / 1000;
}