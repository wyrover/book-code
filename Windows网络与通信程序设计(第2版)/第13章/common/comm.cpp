//////////////////////////////////////////////////
// comm.cpp文件


#include <winsock2.h>
#include <windows.h>
#include "Ws2tcpip.h"

#include "comm.h"


USHORT checksum(USHORT* buff, int size)
{
	unsigned long cksum = 0;
	while(size>1)
	{
		cksum += *buff++;
		size -= sizeof(USHORT);
	}
	// 是奇数
	if(size)
	{
		cksum += *(UCHAR*)buff;
	}
	// 将32位的chsum高16位和低16位相加，然后取反
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);			
	return (USHORT)(~cksum);
}

BOOL SetTTL(SOCKET s, int nValue)
{
	int ret = ::setsockopt(s, IPPROTO_IP, IP_TTL, (char*)&nValue, sizeof(nValue));
	return ret != SOCKET_ERROR;
}

BOOL SetTimeout(SOCKET s, int nTime, BOOL bRecv)
{
	int ret = ::setsockopt(s, SOL_SOCKET, 
		bRecv ? SO_RCVTIMEO : SO_SNDTIMEO, (char*)&nTime, sizeof(nTime));
	return ret != SOCKET_ERROR;
}
