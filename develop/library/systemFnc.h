#include "systemOS.h"
#ifndef LIB_SYSTEMS_H_
#define LIB_SYSTEMS_H_

#ifdef SYS_LINUX
    #include <unistd.h>
    #define PAUSE printf("Press Enter key to continue..."); fgetc(stdin);
    #define SLEEP(ms) usleep(ms*1000);
#endif
#ifdef SYS_WINDOWS
    #define PAUSE system("pause");
    #define SLEEP(ms) Sleep(ms*1000);

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

        public:
            SYS(char* window);

            RECT rctScreen, MSxy;

            static void GetFocusWindowText();
            static void SaveToClipboard(const char* str);
            static int gettimeofday(struct timeval * tp, struct timezone * tzp);
            static std::string gettimestring();
            
            SYS setWindow(char* window);
            bool isEnable();
            void keybd(const char* btn, char type);
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
            void focus();
            void pause();
    };
#endif

#endif