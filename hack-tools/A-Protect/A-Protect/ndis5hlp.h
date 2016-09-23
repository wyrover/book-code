#ifndef _NDIS5HLP_H
#define _NDIS5HLP_H 1

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#include "..\TcpSnifferDriver\ndis5pkt.h"

HANDLE PacketOpenAdapter(LPCSTR lpAdapterName, BOOL bOverlapped = FALSE);
void PacketCloseAdapter(HANDLE hDevice);
BOOL PacketSetPacketFilter(HANDLE hDevice, DWORD dwFilter);
BOOL PacketQueryStatistics(HANDLE hDevice, STATISTICS_DATA *sd);

inline
BOOL PacketRead(HANDLE hDevice, LPVOID lpBuffer, LPDWORD lpLength, LPOVERLAPPED lpOverlapped = NULL)
{
	return ReadFile(hDevice, lpBuffer, *lpLength, lpLength, lpOverlapped);
//	return DeviceIoControl(hDevice, IOCTL_NDIS5PKT_READ, NULL, 0, lpBuffer, *lpLength, lpLength, lpOverlapped);
}

inline
BOOL PacketWrite(HANDLE hDevice, LPVOID lpBuffer, LPDWORD lpLength, LPOVERLAPPED lpOverlapped = NULL)
{
	return WriteFile(hDevice, lpBuffer, *lpLength, lpLength, lpOverlapped);
}

#endif