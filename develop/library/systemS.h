#ifndef LIB_SYSTEMS_H_
#define LIB_SYSTEMS_H_

#ifdef SYS_LINUX
    #define PAUSE printf("Press Enter key to continue..."); fgetc(stdin);
#endif
#ifdef SYS_WINDOWS
    #define PAUSE system("pause");
#endif

#endif