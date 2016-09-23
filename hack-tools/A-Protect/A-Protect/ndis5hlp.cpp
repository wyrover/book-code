#include "stdafx.h"
#include "ndis5hlp.h"

HANDLE PacketOpenAdapter(LPCSTR lpAdapterName, BOOL bOverlapped)
{
	HANDLE hDevice;
	DWORD dwReturn;
	BOOL bResult;
	DWORD dwFlag;

	if (bOverlapped == TRUE)
		dwFlag = FILE_FLAG_OVERLAPPED;
	else
		dwFlag = 0;

	char szAdapterDevice[MAX_ADAPTER_NAME_LENGTH + 16] = "\\Device\\";
	strcat(szAdapterDevice, lpAdapterName);

	hDevice = CreateFileA(NDIS5PKT_DEVICE_NAME_WIN32,
		GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, dwFlag, 0);
	if (hDevice == INVALID_HANDLE_VALUE)
		return hDevice;

	bResult = DeviceIoControl(hDevice, IOCTL_NDIS5PKT_BIND_ADAPTER,
		szAdapterDevice, strlen(szAdapterDevice)+1, 0, 0, &dwReturn, 0);
	ASSERT(bResult == TRUE);

	dwReturn = GetLastError();

	if (!bResult)
	{
		CloseHandle(hDevice);
		hDevice = INVALID_HANDLE_VALUE;

		SetLastError(dwReturn);
	}

	return hDevice;
}

void PacketCloseAdapter(HANDLE hDevice)
{
	CloseHandle(hDevice);
}

BOOL PacketQueryStatistics(HANDLE hDevice, STATISTICS_DATA *sd)
{
	DWORD dwReturn;
	return DeviceIoControl(hDevice, IOCTL_NDIS5PKT_QUERY_STATISTICS,
		NULL, 0, sd, sizeof(STATISTICS_DATA), &dwReturn, 0);
}

/*
//
// Ndis Packet Filter Bits (OID_GEN_CURRENT_PACKET_FILTER).
//
#define NDIS_PACKET_TYPE_DIRECTED               0x00000001
#define NDIS_PACKET_TYPE_MULTICAST              0x00000002
#define NDIS_PACKET_TYPE_ALL_MULTICAST          0x00000004
#define NDIS_PACKET_TYPE_BROADCAST              0x00000008
#define NDIS_PACKET_TYPE_SOURCE_ROUTING         0x00000010
#define NDIS_PACKET_TYPE_PROMISCUOUS            0x00000020
#define NDIS_PACKET_TYPE_SMT                    0x00000040
#define NDIS_PACKET_TYPE_ALL_LOCAL              0x00000080
#define NDIS_PACKET_TYPE_GROUP                  0x00001000
#define NDIS_PACKET_TYPE_ALL_FUNCTIONAL         0x00002000
#define NDIS_PACKET_TYPE_FUNCTIONAL             0x00004000
#define NDIS_PACKET_TYPE_MAC_FRAME              0x00008000
*/

BOOL PacketSetPacketFilter(HANDLE hDevice, DWORD dwFilter)
{
	DWORD dwReturn;
	PACKET_OID_DATA pod;
	pod.Oid = OID_GEN_CURRENT_PACKET_FILTER;
	*((PLONG)pod.Data) = dwFilter;
	return DeviceIoControl(hDevice, IOCTL_NDIS5PKT_SET_OID_VALUE,
		&pod, sizeof(pod), &pod, sizeof(pod), &dwReturn, 0);
}

/*
VOID PacketCloseAdapter(LPADAPTER lpAdapter)
{
	CloseHandle(lpAdapter->hFile);
}

BOOLEAN PacketSetReadTimeout(LPADAPTER AdapterObject,int timeout)
{
	return TRUE;
}

BOOLEAN PacketSetMinToCopy(LPADAPTER AdapterObject,int nbytes)
{
	return TRUE;
}

BOOLEAN PacketSetBuff(LPADAPTER AdapterObject,int dim)
{
	return TRUE;
}

VOID PacketInitPacket(LPPACKET lpPacket,PVOID  Buffer,UINT  Length)
{
}

LPPACKET PacketAllocatePacket(void)
{
	return new PACKET;
}

BOOLEAN PacketSetHwFilter(LPADAPTER AdapterObject,ULONG Filter)
{
}

BOOLEAN PacketGetNetType (LPADAPTER AdapterObject,NetType *type)
{
	return TRUE;
}

LPADAPTER PacketOpenAdapter(PCHAR AdapterName)
{
	DWORD dwReturn;
	LPADAPTER adapter = new ADAPTER;
	adapter->hFile = CreateFile(NDIS5PKT_DEVICE_NAME_WIN32,
		GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	DeviceIoControl(adapter->hFile, IOCTL_NDIS5PKT_BIND_ADAPTER,
		AdapterName, strlen(AdapterName)+1, 0, 0, &dwReturn, 0);
	DeviceIoControl(adapter->hFile, IOCTL_NDIS5PKT_OPEN_DEVICE,
		AdapterName, strlen(AdapterName)+1, 0, 0, &dwReturn, 0);

	return adapter;
}

BOOLEAN PacketReceivePacket(LPADAPTER AdapterObject,LPPACKET lpPacket,BOOLEAN Sync)
{
	return ReadFile(AdapterObject->hFile, lpPacket->Buffer, lpPacket->Length, &lpPacket->ulBytesReceived, 0);
}

BOOLEAN PacketSetBpf(LPADAPTER AdapterObject,struct bpf_program *fp)
{
	return TRUE;
}

int main2(int argc, char* argv[])
{
	DWORD dwReturn;

	HANDLE hDevice = CreateFile(NDIS5PKT_DEVICE_NAME_WIN32,
		GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	DeviceIoControl(hDevice, IOCTL_NDIS5PKT_BIND_ADAPTER, L"\\Device\\{CCFB71CF-D487-464B-9A23-7A4586853F38}",
		sizeof(L"\\Device\\{CCFB71CF-D487-464B-9A23-7A4586853F38}"), 0, 0, &dwReturn, 0);
	DeviceIoControl(hDevice, IOCTL_NDIS5PKT_OPEN_DEVICE, L"\\Device\\{CCFB71CF-D487-464B-9A23-7A4586853F38}",
		sizeof(L"\\Device\\{CCFB71CF-D487-464B-9A23-7A4586853F38}"), 0, 0, &dwReturn, 0);
	
	PACKET_OID_DATA pod;
	pod.Oid = OID_GEN_CURRENT_PACKET_FILTER;
	*((PLONG)pod.Data) = NDIS_PACKET_TYPE_PROMISCUOUS; //NDIS_PACKET_TYPE_ALL_LOCAL; //NDIS_PACKET_TYPE_DIRECTED; //;
	dwReturn = DeviceIoControl(hDevice, IOCTL_NDIS5PKT_SET_OID_VALUE,
		&pod, sizeof(pod), &pod, sizeof(pod), &dwReturn, 0);

	pod.Oid = OID_GEN_MAXIMUM_TOTAL_SIZE;
	dwReturn = DeviceIoControl(hDevice, IOCTL_NDIS5PKT_QUERY_OID_VALUE,
		&pod, sizeof(pod), &pod, sizeof(pod), &dwReturn, 0);

	pod.Oid = OID_GEN_MAXIMUM_FRAME_SIZE;
	dwReturn = DeviceIoControl(hDevice, IOCTL_NDIS5PKT_QUERY_OID_VALUE,
		&pod, sizeof(pod), &pod, sizeof(pod), &dwReturn, 0);
	
	if (hDevice != INVALID_HANDLE_VALUE)
	{
		BYTE buf[1514];
//		OVERLAPPED overlapped[16];
//		HANDLE e[16];
		LONGLONG count = 0;
//
//		for (int i=0; i<16; i++)
//		{
//			overlapped[i].Offset = 0;
//			overlapped[i].OffsetHigh = 0;
//			e[i] = overlapped[i].hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//			ReadFile(hDevice, buf[i], 1514, NULL, &overlapped[i]);
//		}

		while (1)
		{
//			DWORD ret = WaitForMultipleObjects(16, e, FALSE, 1000);
//			if (ret >= WAIT_OBJECT_0 && ret < WAIT_OBJECT_0 + 16)
//			{
//				count++;
//				ResetEvent(e[i]);
//				ReadFile(hDevice, buf[ret - WAIT_OBJECT_0], 1514, NULL, &overlapped[ret - WAIT_OBJECT_0]);
//
//				BYTE *p = buf[ret - WAIT_OBJECT_0];
//				printf("%d: %02X:%02X:%02X:%02X:%02X:%02X -> %02X:%02X:%02X:%02X:%02X:%02X\n",
//					ret - WAIT_OBJECT_0,
//					p[6], p[7], p[8], p[9], p[10], p[11],
//					p[0], p[1], p[2], p[3], p[4], p[5]);
//			}
			if (ReadFile(hDevice, buf, 1514, &dwReturn, 0))
			{
//				printf("P: %02X:%02X:%02X:%02X:%02X:%02X -> %02X:%02X:%02X:%02X:%02X:%02X\n",
//					buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
//					buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
			}
			count++;
			if (count%100000 == 0)
			{
				STATISTICS_DATA stat;
				DeviceIoControl(hDevice, IOCTL_NDIS5PKT_QUERY_STATISTICS,
					NULL, 0, &stat, sizeof(stat), &dwReturn, 0);
				printf("Rx: %I64d, Drop: %I64d\n", stat.ReceivedPackets, stat.DroppedPackets);
			}
		}
//		while (1)
//		{
//			DWORD dwRead;
//			BYTE buf[1514];
//			if (ReadFile(hDevice, buf, 1514, &dwRead, 0))
//			{
//				printf("P: %02X:%02X:%02X:%02X:%02X:%02X -> %02X:%02X:%02X:%02X:%02X:%02X\n",
//					buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
//					buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
//			}
//		}
	}
	//CloseHandle(hDevice);
	return 0;
}

*/