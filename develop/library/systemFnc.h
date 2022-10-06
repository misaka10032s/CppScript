#include "systemOS.h"
#ifndef LIB_SYSTEMS_H_
#define LIB_SYSTEMS_H_

#ifdef SYS_LINUX
    #include <unistd.h>
    #define PAUSE printf("Press Enter key to continue..."); fgetc(stdin);
    #define SLEEP(ms) usleep(ms*1000);
#endif
#ifdef SYS_WINDOWS
// #elif defined SYS_WINDOWS
    #define PAUSE system("pause");
    #define SLEEP(ms) Sleep(ms);

    #include <map>
    #include <windows.h>
    #include <iostream>

    class SYS{
        private:
            struct cmp_str{
                bool operator()(char const *a, char const *b) const
                {
                    return strcmp(a, b) < 0;
                }
            };

            std::map<const char*, int, cmp_str> key;
            HWND targetWND;
            struct timeval timestamp;
            bool enable, check_WHD=1;
            unsigned char switchKey, maximizeKey, minimizeKey, focusKey, setForegroundWindow;

        public:
            SYS(const char* window);

            RECT rctScreen, targetWNDsize;

            static void GetFocusWindowText();
            static void SaveToClipboard(const char* str);
            static int gettimeofday(struct timeval * tp, struct timezone * tzp);
            static std::string gettimestring();
            
            SYS setWindow(const char* window);
            SYS setWindow();
            void setKey(unsigned char key, unsigned char value);
            bool isEnable();
            void keybd(const char* btn, char type);
            void mouseAction(int x, int y, char type, bool abs, DWORD dwFlags);
            // mouse position absolute
            void mouseRC(int x, int y, char type);
            void mouseLC(int x, int y, char type);
            void mouseMC(int x, int y, char type);
            void mouseWH(int x, int y, char type);
            void mouseMV(int x, int y);
            // mouse position relative
            void mouseRCr(int dx, int dy, char type);
            void mouseLCr(int dx, int dy, char type);
            void mouseMCr(int dx, int dy, char type);
            void mouseWHr(int dx, int dy, char type);
            void mouseMVr(int dx, int dy);
            void wait(int dur); // dur ms
            void wait(int minDur, int rangeDur); // dur ms
            void minimize();
            void maximize();
            void focus();
            void pause();
    };
#endif

#endif