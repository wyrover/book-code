////////////////////////////////////////////
// protoutils.h文件




#ifndef __PROTOUTILS_H__
#define __PROTOUTILS_H__

#include "nuiouser.h"

//////////////////////////////////////
// 函数定义

BOOL ProtoStartService();
void ProtoStopService();
HANDLE ProtoOpenControlDevice();


//////////////////////////////////////
// CPROTOAdapters类，枚举绑定的适配器

#define MAX_ADAPTERS 10

class CPROTOAdapters
{
public:
	BOOL EnumAdapters(HANDLE hControlDevice);
	
	int m_nAdapters;
	LPWSTR m_pwszAdapterName[MAX_ADAPTERS];
	LPWSTR m_pwszSymbolicLink[MAX_ADAPTERS];
protected:
	char m_buffer[MAX_ADAPTERS*256];
};

////////////////////////////////////////
// CAdapter类，管理绑定的适配器

class CAdapter
{
public:
	CAdapter();
	~CAdapter();

	// 打开、关闭适配器
	BOOL OpenAdapter(LPCWSTR pwszSymbolicLink, BOOL bAsyn = FALSE);
	void CloseAdapter();

	// 设置过滤属性，如NDIS_PACKET_TYPE_PROMISCUOUS、NDIS_PACKET_TYPE_DIRECTED等
	BOOL SetFilter(ULONG nFilters);

	// 接收、发送数据
	int RecieveData(PVOID pBuffer, int nLen, LPOVERLAPPED lpOverlapped = NULL);
	int SendData(PVOID pBuffer, int nLen, LPOVERLAPPED lpOverlapped = NULL);

	// 重启下层NIC、管理OID信息
	BOOL ResetAdapter();
	BOOL ProtoRequest(PPROTOCOL_OID_DATA pOidData, BOOL bQuery);

protected:
	HANDLE m_hAdapter;
};


#endif // __PROTOUTILS_H__