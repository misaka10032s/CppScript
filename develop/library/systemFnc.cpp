#include "systemOS.h"
#include "systemFnc.h"
#include "systemFncScanCode.h"
#include <string>

    // VK key list
    // http://www.kbdedit.com/manual/low_level_vk_list.html
    // 键盘扫描码
    // https://blog.csdn.net/RopenYuan/article/details/42298195
    // INPUT struct
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-input

#if defined(SYS_LINUX)
    
#elif defined(SYS_WINDOWS)
    #include <map>
    #include <windows.h>
    #include <iostream>

    struct cmp_str{
        bool operator()(char const *a, char const *b) const
        {
            return strcmp(a, b) < 0;
        }
    };

    std::map<const char*, int, cmp_str> keyCode;
    void set_key_value(){
        for (int i = 2; i < 12; i++) {
            const char* a[] = { "1","2","3","4","5","6","7","8","9","0" };
            keyCode[a[i - 2]] = i;
        }
        for (int i = 16; i < 26; i++) {
            const char* a[] = { "Q","W","E","R","T","Y","U","I","O","P" };
            keyCode[a[i - 16]] = i;
        }
        for (int i = 30; i < 39; i++) {
            const char* a[] = { "A","S","D","F","G","H","J","K","L" };
            keyCode[a[i - 30]] = i;
        }
        for (int i = 44; i < 51; i++) {
            const char* a[] = { "Z","X","C","V","B","N","M" };
            keyCode[a[i - 44]] = i;
        }
        for (int i = 59; i < 69; i++) {
            const char* a[] = { "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10" };
            keyCode[a[i - 59]] = i;
        }
        keyCode["F11"] = 87;
        keyCode["F12"] = 88;

        keyCode["ESC"] = 1;
        keyCode["-"] = 12;
        keyCode["="] = 13;
        keyCode["BACKSPACE"] = 14;
        keyCode["TAB"] = 15;
        keyCode["ENTER"] = 28;
        keyCode["CTRL"] = 29;
        keyCode[";"] = 39;
        keyCode["\'"] = 40;
        keyCode["`"] = 41;
        keyCode["LSHIFT"] = 42;
        keyCode["\\"] = 43;
        keyCode["<"] = 51;
        keyCode[","] = 51;
        keyCode[">"] = 52;
        keyCode["."] = 52;
        keyCode["/"] = 53;
        keyCode["RSHIFT"] = 54;
        keyCode["*"] = 55;
        keyCode["ALT"] = 56;
        keyCode["SPACE"] = 57;
        keyCode["CAPSLOCK"] = 58;
        keyCode["NUMLOCK"] = 69;
        keyCode["SCRLOCK"] = 70;

        keyCode["LEFT"] = 75;
        keyCode["RIGHT"] = 77;
        keyCode["UP"] = 72;
        keyCode["DOWN"] = 80;

        keyCode["PGU"] = 201;
        keyCode["PGD"] = 209;
        keyCode["INS"] = 210;
        keyCode["DEL"] = 211;
        keyCode["HOME"] = 199;
        keyCode["END"] = 207;
    };

    SYS::SYS(const char* window){
        GetWindowRect(FindWindow(NULL, (LPCSTR)window), &this->targetWNDsize);
        GetWindowRect(GetDesktopWindow(), &this->rctScreen);
        this->targetWND = FindWindow(NULL, (LPCSTR)window);

        this->enable = 0;

        this->switchKey = VK_F10;
        this->maximizeKey = VK_F3;
        this->minimizeKey = VK_F4;
        this->focusKey = VK_F5;
        this->setForegroundWindow = VK_F2;

        this->timeCountDown = -1;
        set_key_value();
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


    SYS SYS::setWindow(const char* window){
        this->targetWND = FindWindow(NULL, (LPCSTR)window);
        return *this;
    };
    SYS SYS::setWindow(){
        this->targetWND = GetForegroundWindow();
        return *this;
    };
    void SYS::setKey(unsigned char key, unsigned char value){
        switch(key){
            case 0:
                this->switchKey = value;
                break;
            case 1:
                this->maximizeKey = value;
                break;
            case 2:
                this->minimizeKey = value;
                break;
            case 3:
                this->focusKey = value;
                break;
            case 4:
                this->setForegroundWindow = value;
                break;
        }
    };

    bool SYS::isEnable(){
        return this->enable;
    };

    long int SYS::getNowtick(){
        gettimeofday(&this->timestamp, NULL);
        return this->timestamp.tv_sec * 1000 + this->timestamp.tv_usec / 1000;
    };

    void SYS::keybd(const char* btn, char type){
        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.time = 0;
        ip.ki.wVk = 0;
        ip.ki.dwExtraInfo = 0;
        ip.ki.wScan = keyCode[btn];

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

    void SYS::mouseAction(int x, int y, char type, bool abs, DWORD dwFlags){
        GetWindowRect(this->targetWND, &this->targetWNDsize);
        mouse_event((abs ? MOUSEEVENTF_ABSOLUTE : 0) | MOUSEEVENTF_MOVE, (this->targetWNDsize.left + x) * 65536 / this->rctScreen.right, (this->targetWNDsize.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        SLEEP(10);
        if(type & 1) mouse_event(dwFlags, 0, 0, 0, 0);
        if(type & 3) SLEEP(30);
        if(type & 2) mouse_event(dwFlags<<1, 0, 0, 0, 0);
    };

    void SYS::mouseRC(int x, int y, char type){
        this->mouseAction(x, y, type, 1, MOUSEEVENTF_RIGHTDOWN);
        // GetWindowRect(this->targetWND, &this->targetWNDsize);
        // mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->targetWNDsize.left + x) * 65536 / this->rctScreen.right, (this->targetWNDsize.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        // SLEEP(10);
        // if(type & 1) {
        //     mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        // }
        // if(type & 3) SLEEP(30);
        // if(type & 2) {
        //     mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        // }
    };
    void SYS::mouseLC(int x, int y, char type){
        this->mouseAction(x, y, type, 1, MOUSEEVENTF_LEFTDOWN);
        // GetWindowRect(this->targetWND, &this->targetWNDsize);
        // mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->targetWNDsize.left + x) * 65536 / this->rctScreen.right, (this->targetWNDsize.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        // SLEEP(10);
        // if(type & 1) {
        //     mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        // }
        // if(type & 3) SLEEP(30);
        // if(type & 2) {
        //     mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        // }
    };
    void SYS::mouseMC(int x, int y, char type){
        this->mouseAction(x, y, type, 1, MOUSEEVENTF_MIDDLEDOWN);
        // GetWindowRect(this->targetWND, &this->targetWNDsize);
        // mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->targetWNDsize.left + x) * 65536 / this->rctScreen.right, (this->targetWNDsize.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
        // SLEEP(10);
        // if(type & 1) {
        //     mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
        // }
        // if(type & 3) SLEEP(30);
        // if(type & 2) {
        //     mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
        // }
    };
    void SYS::mouseWH(int x, int y, char type){
        GetWindowRect(this->targetWND, &this->targetWNDsize);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->targetWNDsize.left + x) * 65536 / this->rctScreen.right, (this->targetWNDsize.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
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
        this->mouseAction(x, y, 0, 1, 0);
        GetWindowRect(this->targetWND, &this->targetWNDsize);
	    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (this->targetWNDsize.left + x) * 65536 / this->rctScreen.right, (this->targetWNDsize.top + y) * 65536 / this->rctScreen.bottom, 0, 0);
    };
    // mouse position relative
    void SYS::mouseRCr(int dx, int dy, char type){
        this->mouseAction(dx, dy, type, 0, MOUSEEVENTF_RIGHTDOWN);
        // GetWindowRect(this->targetWND, &this->targetWNDsize);
        // mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        // SLEEP(10);
        // if(type & 1) {
        //     mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        // }
        // if(type & 3) SLEEP(30);
        // if(type & 2) {
        //     mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        // }
    };
    void SYS::mouseLCr(int dx, int dy, char type){
        this->mouseAction(dx, dy, type, 0, MOUSEEVENTF_LEFTDOWN);
        // GetWindowRect(this->targetWND, &this->targetWNDsize);
        // mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        // SLEEP(10);
        // if(type & 1) {
        //     mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        // }
        // if(type & 3) SLEEP(30);
        // if(type & 2) {
        //     mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        // }
    };
    void SYS::mouseMCr(int dx, int dy, char type){
        this->mouseAction(dx, dy, type, 0, MOUSEEVENTF_MIDDLEDOWN);
        // GetWindowRect(this->targetWND, &this->targetWNDsize);
        // mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
        // SLEEP(10);
        // if(type & 1) {
        //     mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
        // }
        // if(type & 3) SLEEP(30);
        // if(type & 2) {
        //     mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
        // }
    };
    void SYS::mouseWHr(int dx, int dy, char type){
        GetWindowRect(this->targetWND, &this->targetWNDsize);
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
        this->mouseAction(dx, dy, 0, 0, 0);
        GetWindowRect(this->targetWND, &this->targetWNDsize);
	    mouse_event(MOUSEEVENTF_MOVE, dx * 65536 / this->rctScreen.right, dy * 65536 / this->rctScreen.bottom, 0, 0);
    };


    void SYS::wait(int dur){
        gettimeofday(&this->timestamp, NULL);
        long int st = this->timestamp.tv_sec * 1000 + this->timestamp.tv_usec / 1000, ed;
        bool frontIsTarget, numLockOn;

        if(this->timeCountDown != -1 && this->timeCountDown <= st){
            this->enable = 0;
            this->timeCountDown = -1;
        }
        while(1){
            gettimeofday(&this->timestamp, NULL);
            frontIsTarget = GetForegroundWindow() == this->targetWND;
            numLockOn = LOWORD(GetKeyState(VK_NUMLOCK));
            ed = this->timestamp.tv_sec * 1000 + this->timestamp.tv_usec / 1000;
            // std::cout<<this->enable<<", "<<GetForegroundWindow()<<", "<<this->targetWND<<", "<<frontIsTarget<<", "<<numLockOn<<"\n";
            if(ed - st > dur && this->enable && (frontIsTarget || !this->check_WHD) && (this->timeCountDown == -1 || this->timeCountDown > st)){
                if(ed - st > dur + 1000) SLEEP(100);
                break;
            }
            if (GetAsyncKeyState(this->switchKey) < 0) {
                this->enable ^= 1; 
                if(this->enable == numLockOn && frontIsTarget){
                    keybd("NUMLOCK", 3);
                    SLEEP(50);
                }
                std::cout << (this->enable ? "work": "stop")  << "\n";
                SLEEP(200);
            }
            if(GetAsyncKeyState(this->maximizeKey) < 0) this->maximize();
            if(GetAsyncKeyState(this->minimizeKey) < 0) this->minimize();
            if(GetAsyncKeyState(this->focusKey) < 0) this->focus();
            if(GetAsyncKeyState(this->setForegroundWindow) < 0) this->setWindow();

            if((!frontIsTarget && !numLockOn) || ((frontIsTarget) && (this->enable) && numLockOn)){
                keybd("NUMLOCK", 3);
                SLEEP(50);
            }

            SLEEP(10);
        }
        gettimeofday(&this->timestamp, NULL);
    };

    void SYS::wait(int minDur, int rangeDur){
        int dur = rand()%rangeDur + minDur;
        this->wait(dur);
    };
    // https://stackoverflow.com/questions/31224092/maximize-minimize-window-from-another-thread
    void SYS::minimize(){
        PostMessage(this->targetWND, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    };
    void SYS::maximize(){
        PostMessage(this->targetWND, WM_SYSCOMMAND, SC_RESTORE, 0);
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
    void SYS::setCountdown(int time){
        gettimeofday(&this->timestamp, NULL);
        this->timeCountDown = this->timestamp.tv_sec * 1000 + this->timestamp.tv_usec / 1000 + time;
    };
#endif