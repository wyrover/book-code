//////////////////////////////////////////////////////////
// initsock.h文件

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib

class CInitSock		
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if(::WSAStartup(sockVersion, &wsaData) != 0)
		{
			exit(0);
		}
	}
	~CInitSock()
	{	
		::WSACleanup();	
	}
};
