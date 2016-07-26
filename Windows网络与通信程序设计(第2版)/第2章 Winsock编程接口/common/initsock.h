//////////////////////////////////////////////////////////
// initsock.h文件

#include <winsock2.h>		//包含头文件
#pragma comment(lib, "WS2_32")	// 添加库WS2_32.lib

class CInitSock		
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if(::WSAStartup(sockVersion, &wsaData) != 0)	//加载Winsock_DLL库,调用成功返回0
		{
			exit(0);
		}
	}
	~CInitSock()
	{	
		::WSACleanup();		//释放Winsock库
	}
};

// int
// WSAAPI
// WSAStartup(		//加载Winsock_DLL库
// 		   WORD wVersionRequested,
// 		   LPWSADATA lpWSAData
//     );