//std.h

//#define MY_DEBUG

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
using namespace std;
#include "data.h"
#include "myservice.h"
#include "servicehandler.h"

#include "sever.h"
#pragma comment (lib, "sever.lib")

typedef struct tagPacket
{
	int flag;
	char s_name[20];
	char s_time[20];		// 00:00:00 
	char s_message[50];
}PACKET;
