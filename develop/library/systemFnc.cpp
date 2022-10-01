#include "systemOS.h"
#include "systemFnc.h"
#include "systemFncScanCode.h"

    // VK key list
    // http://www.kbdedit.com/manual/low_level_vk_list.html
    // 键盘扫描码
    // https://blog.csdn.net/RopenYuan/article/details/42298195
    // INPUT struct
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-input

#ifdef SYS_LINUX
    
#endif
#ifdef SYS_WINDOWS
    #include <map>
    #include <windows.h>
    #include <iostream>
    SYS::SYS(char* window){
        for (int i = 2; i < 12; i++) {
            const char* a[] = { "1","2","3","4","5","6","7","8","9","0" };
            this->key[a[i - 2]] = i;
        }
        for (int i = 16; i < 26; i++) {
            const char* a[] = { "Q","W","E","R","T","Y","U","I","O","P" };
            this->key[a[i - 16]] = i;
        }
        for (int i = 30; i < 39; i++) {
            const char* a[] = { "A","S","D","F","G","H","J","K","L" };
            this->key[a[i - 30]] = i;
        }
        for (int i = 44; i < 51; i++) {
            const char* a[] = { "Z","X","C","V","B","N","M" };
            this->key[a[i - 44]] = i;
        }
        for (int i = 59; i < 69; i++) {
            const char* a[] = { "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10" };
            this->key[a[i - 59]] = i;
        }
        this->key["F11"] = 87;
        this->key["F12"] = 88;

        this->key["ESC"] = 1;
        this->key["-"] = 12;
        this->key["="] = 13;
        this->key["BACKSPACE"] = 14;
        this->key["TAB"] = 15;
        this->key["ENTER"] = 28;
        this->key["CTRL"] = 29;
        this->key[";"] = 39;
        this->key["\'"] = 40;
        this->key["`"] = 41;
        this->key["LSHIFT"] = 42;
        this->key["\\"] = 43;
        this->key["<"] = 51;
        this->key[","] = 51;
        this->key[">"] = 52;
        this->key["."] = 52;
        this->key["/"] = 53;
        this->key["RSHIFT"] = 54;
        this->key["*"] = 55;
        this->key["ALT"] = 56;
        this->key["SPACE"] = 57;
        this->key["CAPSLOCK"] = 58;
        this->key["NUMLOCK"] = 69;
        this->key["SCRLOCK"] = 70;

        this->key["LEFT"] = 75;
        this->key["RIGHT"] = 77;
        this->key["UP"] = 72;
        this->key["DOWN"] = 80;

        this->key["PGU"] = 201;
        this->key["PGD"] = 209;
        this->key["INS"] = 210;
        this->key["DEL"] = 211;
        this->key["HOME"] = 199;
        this->key["END"] = 207;

        GetWindowRect(FindWindow(NULL, (LPCSTR)window), &this->MSxy);
        GetWindowRect(GetDesktopWindow(), &this->rctScreen);
    }

    void SYS::GetFocusWindowText(){
        HWND wnd = GetForegroundWindow();
        int len = GetWindowTextLength(wnd) + 1;
        std::string s;
        s.reserve(len);
        GetWindowText(wnd, const_cast<char*>(s.c_str()), len - 1);
    }

    void SYS::SaveToClipboard(const char* str){
        if (OpenClipboard(NULL)) {
            HGLOBAL clipbuffer;
            char* buffer;
            EmptyClipboard();
            clipbuffer = GlobalAlloc(GMEM_MOVEABLE, strlen(str) + 1);
            buffer = (char*)GlobalLock(clipbuffer);
            strcpy_s(buffer, strlen(str) + 1, (LPCSTR)str);
            GlobalUnlock(clipbuffer);
            SetClipboardData(CF_TEXT, clipbuffer);
            CloseClipboard();
            GlobalUnlock(clipbuffer);
            GlobalFree(clipbuffer);
        }
    };
    int SYS::gettimeofday(struct timeval * tp, struct timezone * tzp){
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
    };
    std::string SYS::gettimestring(){
        struct timeval tp;
        gettimeofday(&tp, NULL);
        long int num = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        return std::to_string(num);
    };


    SYS SYS::setWindow(char* window){
        this->targetWND = FindWindow(NULL, (LPCSTR)window);
        return *this;
    };

    bool SYS::isEnable(){
        return this->enable;
    };

    void SYS::keybd(const char* btn, char type){
        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.time = 0;
        ip.ki.wVk = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wScan = key[btn];

        if(type & 1) {
            ip.ki.dwFlags = KEYEVENTF_SCANCODE;
            SendInput(1, &ip, sizeof(INPUT));
            //cout<<"DA\t"<<ip.ki.wScan<<"\t"<<type<<endl;
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            SendInput(1, &ip, sizeof(INPUT));
            //cout<<"UA\t"<<ip.ki.wScan<<"\t"<<type<<endl;
        }
    };
    // mouse position absolute
    void SYS::mouseRC(int x, int y, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->MSxy.left + x) * 65536 / this->rctScreen.right, (this->MSxy.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }
    };
    void SYS::mouseLC(int x, int y, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->MSxy.left + x) * 65536 / this->rctScreen.right, (this->MSxy.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    };
    void SYS::mouseMC(int x, int y, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->MSxy.left + x) * 65536 / this->rctScreen.right, (this->MSxy.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
        }
    };
    void SYS::mouseWH(int x, int y, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->MSxy.left + x) * 65536 / this->rctScreen.right, (this->MSxy.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(WHEEL_DELTA), 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(-WHEEL_DELTA), 0);
        }
    };
    void SYS::mouseMV(int x, int y){
        GetWindowRect(this->targetWND, &this->MSxy);
	    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->MSxy.left + x) * 65536 / this->rctScreen.right, (this->MSxy.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
    };
    // mouse position relative
    void SYS::mouseRCr(int dx, int dy, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }
    };
    void SYS::mouseLCr(int dx, int dy, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    };
    void SYS::mouseMCr(int dx, int dy, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
        }
    };
    void SYS::mouseWHr(int dx, int dy, char type){
        GetWindowRect(this->targetWND, &this->MSxy);
        mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(WHEEL_DELTA), 0);
        }
        if(type & 3) SLEEP(30);
        if(type & 2) {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(-WHEEL_DELTA), 0);
        }
    };
    void SYS::mouseMVr(int dx, int dy){
        GetWindowRect(this->targetWND, &this->MSxy);
	    mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
    };


    void SYS::wait(int dur){
        gettimeofday(&this->timestamp, NULL);
        long int st = this->timestamp.tv_sec * 1000 + this->timestamp.tv_usec / 1000, ed;
        bool frontIsTarget, numLockOn;
        while(1){
            gettimeofday(&this->timestamp, NULL);
            frontIsTarget = GetForegroundWindow() == this->targetWND;
            numLockOn = LOWORD(GetKeyState(VK_NUMLOCK));
            ed = this->timestamp.tv_sec * 1000 + this->timestamp.tv_usec / 1000;
            if(ed - st > dur && this->enable && (frontIsTarget || !this->check_WHD)){
                if(ed - st > dur + 1000) SLEEP(100);
                break;
            }
            if (GetAsyncKeyState(VK_F10) < 0) {
                this->enable ^= 1; 
                if(this->enable == numLockOn && frontIsTarget){
                    keybd("NUMLOCK", 3);
                    SLEEP(50);
                }
                std::cout << (this->enable ? "work": "stop")  << "\n";
                SLEEP(200);
            }

            if((!frontIsTarget && !numLockOn) || ((frontIsTarget) && (this->enable) && numLockOn)){
                keybd("NUMLOCK", 3);
                SLEEP(50);
            }

            SLEEP(10);
        }
        gettimeofday(&this->timestamp, NULL);
    };
    void SYS::focus(){
        SetForegroundWindow(this->targetWND);
    };
    void SYS::pause(){
        this->enable = 0;
        if(!LOWORD(GetKeyState(VK_NUMLOCK))){
            keybd("NUMLOCK", 3);
            SLEEP(50);
        }
    };
#endif