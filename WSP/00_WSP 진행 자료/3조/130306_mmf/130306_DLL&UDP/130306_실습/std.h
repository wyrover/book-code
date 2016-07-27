//std.h

//#define MY_DEBUG

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
using namespace std;

typedef struct tagPacket
{
	int flag;
	char s_name[20];
	char s_time[20];		// 00:00:00 
	char s_message[50];
}PACKET;


#include "handler.h"
#include "resource.h"
#include "ui.h"
//#include "sever.h"
//#pragma comment (lib, "sever.lib")
#include "client.h"
#pragma comment (lib, "client.lib")

extern int g_group;
extern int g_groupsend;
extern char g_name[20];
extern TCHAR message[50];

extern PACKET pack;
extern HWND g_hDlg;




