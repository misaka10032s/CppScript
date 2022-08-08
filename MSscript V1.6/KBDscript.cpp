#include <map>
#include <windows.h>
#include <iostream>
using namespace std;

RECT rctScreen, MSxy;
// #define VK_NUMLOCK 144

void set_key_value();
void keybd(const char* btn, const char* type, int dur);
void SaveToClipboard(const char* str);
void click(int x, int y, const char* type);

struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return strcmp(a, b) < 0;
	}
};

map<const char*, int, cmp_str> key;

void set_key_value() 
{
	//https://blog.csdn.net/RopenYuan/article/details/42298195
	for (int i = 2; i < 12; i++) {
		const char* a[] = { "1","2","3","4","5","6","7","8","9","0" };
		key[a[i - 2]] = i;
	}
	for (int i = 16; i < 26; i++) {
		const char* a[] = { "Q","W","E","R","T","Y","U","I","O","P" };
		key[a[i - 16]] = i;
	}
	for (int i = 30; i < 39; i++) {
		const char* a[] = { "A","S","D","F","G","H","J","K","L" };
		key[a[i - 30]] = i;
	}
	for (int i = 44; i < 51; i++) {
		const char* a[] = { "Z","X","C","V","B","N","M" };
		key[a[i - 44]] = i;
	}
	for (int i = 59; i < 69; i++) {
		const char* a[] = { "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10" };
		key[a[i - 59]] = i;
	}
	key["F11"] = 87;
	key["F12"] = 88;

	key["ESC"] = 1;
	key["-"] = 12;
	key["="] = 13;
	key["BACKSPACE"] = 14;
	key["TAB"] = 15;
	key["ENTER"] = 28;
	key["CTRL"] = 29;
	key[";"] = 39;
	key["\'"] = 40;
	key["`"] = 41;
	key["LSHIFT"] = 42;
	key["\\"] = 43;
	key[","] = 51;
	key["."] = 52;
	key["/"] = 53;
	key["RSHIFT"] = 54;
	key["*"] = 55;
	key["ALT"] = 56;
	key["SPACE"] = 57;
	key["CAPSLOCK"] = 58;
	key["NUMLOCK"] = 69;
	key["SCRLOCK"] = 70;

	key["LEFT"] = 75;
	key["RIGHT"] = 77;
	key["UP"] = 72;
	key["DOWN"] = 80;

	key["PGU"] = 201;
	key["PGD"] = 209;
	key["INS"] = 210;
	key["DEL"] = 211;
	key["HOME"] = 199;
	key["END"] = 207;

    GetWindowRect(FindWindow(NULL, (LPCSTR)"MapleStory"), &MSxy);
	GetWindowRect(GetDesktopWindow(), &rctScreen);
}

void keybd(const char* btn, const char* type) 
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wScan = key[btn];

	if (type == (string)"DOWN" || type == (string)"ALL") {
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
		SendInput(1, &ip, sizeof(INPUT));
		//cout<<"DA\t"<<ip.ki.wScan<<"\t"<<type<<endl;
	}
	if(type == (string)"ALL") Sleep(30);
	if (type == (string)"UP" || type == (string)"ALL") {
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
		//cout<<"UA\t"<<ip.ki.wScan<<"\t"<<type<<endl;
	}
}

// void keybdduring(const char* btn, int dur, bool *enable) 
// {
// 	struct timeval timestamp;
// 	gettimeofday(&timestamp, NULL);
// 	long int st = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000, ed;
// 	while(1){
// 		if((*enable))keybd(btn, "ALL");
// 		gettimeofday(&timestamp, NULL);
//         ed = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
// 		Sleep(50);
// 		if(ed - st > dur) break;
// 	}
// }

void click(int x, int y, const char* type) 
{
	GetWindowRect(FindWindow(NULL, (LPCSTR)"MapleStory"), &MSxy);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (MSxy.left + x) * 65536 / rctScreen.right, (MSxy.top + y) * 65536 / rctScreen.bottom, 0, 0);
	Sleep(20);
	if (type == (string)"DOWN" || type == (string)"ALL") {
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	}
	if(type == (string)"ALL") Sleep(30);
	if (type == (string)"UP" || type == (string)"ALL") {
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}

void SaveToClipboard(const char* str) 
{
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
}