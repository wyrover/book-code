//////////////////////////////////////////////////////////
// NetTime.cpp文件


#include "../common/InitSock.h"
#include <stdio.h>
CInitSock initSock;	

void SetTimeFromTP(ULONG ulTime)	// 根据时间协议返回的时间设置系统时间
{
	// Windows文件时间是一个64位的值，它是从1601年1月1日中午12:00到现在的时间间隔，
	// 单位是1/1000 0000秒，即1000万分之1秒（100-nanosecond ）
	FILETIME ft;
	SYSTEMTIME st;

	// 首先将基准时间（1900年1月1日0点0分0秒0毫秒）转化为Windows文件时间	
	st.wYear = 1900;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;
	SystemTimeToFileTime(&st, &ft);

	// 然后将Time Protocol使用的基准时间加上以及逝去的时间，即ulTime
	LONGLONG *pLLong = (LONGLONG *)&ft;
	// 注意，文件时间单位是1/1000 0000秒，即1000万分之1秒（100-nanosecond ）
	*pLLong += (LONGLONG)10000000 * ulTime; 

	// 再将时间转化回来，更新系统时间
	FileTimeToSystemTime(&ft, &st);	
	SetSystemTime(&st);
}

int main()
{
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 填写远程地址信息，连接到时间服务器
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(37); 
	// 这里使用的时间服务器是129.132.2.21，更多地址请参考http://tf.nist.gov/service/its.htm
	servAddr.sin_addr.S_un.S_addr = inet_addr("129.132.2.21");
	if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		getchar();
		return 0;
	}
	
	// 等待接收时间协议返回的时间。学习了Winsock I/O模型之后，最好使用异步I/O，以便设置超时
	ULONG ulTime = 0;
	int nRecv = ::recv(s, (char*)&ulTime, sizeof(ulTime), 0);
	if(nRecv > 0)
	{
		ulTime = ntohl(ulTime);
		SetTimeFromTP(ulTime);
		printf(" 成功与时间服务器的时间同步！\n");
	}
	else
	{
		printf(" 时间服务器不能确定当前时间！\n");
	}
	
	::closesocket(s);
	getchar();
	return 0;
}


