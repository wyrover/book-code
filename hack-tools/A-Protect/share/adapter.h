#ifndef _ADAPTER_H
#define _ADAPTER_H 1

#ifndef __IPHLPAPI_H__
	#include <Iphlpapi.h>
	#pragma comment(lib, "Iphlpapi.lib")
#endif

#ifndef _ASSERT_H
	#include "assert.h"
#endif

/*
//
// ADAPTER_INFO - per-adapter information. All IP addresses are stored as
// strings
//

typedef struct _IP_ADAPTER_INFO {
    struct _IP_ADAPTER_INFO* Next;
    DWORD ComboIndex;
    char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
    char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
    UINT AddressLength;
    BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
    DWORD Index;
    UINT Type;
    UINT DhcpEnabled;
    PIP_ADDR_STRING CurrentIpAddress;
    IP_ADDR_STRING IpAddressList;
    IP_ADDR_STRING GatewayList;
    IP_ADDR_STRING DhcpServer;
    BOOL HaveWins;
    IP_ADDR_STRING PrimaryWinsServer;
    IP_ADDR_STRING SecondaryWinsServer;
    time_t LeaseObtained;
    time_t LeaseExpires;
} IP_ADAPTER_INFO, *PIP_ADAPTER_INFO;
*/

class CAdapter
{
// Constructor
public:
	CAdapter();
	BOOL Create();

// Attributes
private:
	PIP_ADAPTER_INFO m_pAdapterInfo;
	int m_iAdapterCount;

// Operations
public:
	void Destroy();
	PIP_ADAPTER_INFO operator [](int i) const;
	PIP_ADAPTER_INFO GetAt(int i) const;
	int GetCount() const;

// Overridable callbacks
protected:	

// Implementation
public:
	~CAdapter();
};

inline
CAdapter::CAdapter()
{
	m_pAdapterInfo = NULL;
	m_iAdapterCount = 0;
}

inline
CAdapter::~CAdapter()
{
	Destroy();
}

inline
BOOL CAdapter::Create()
{
	ASSERT(m_pAdapterInfo == NULL);

	ULONG OutBufLen = 0;

	if (GetAdaptersInfo(NULL, &OutBufLen) != ERROR_BUFFER_OVERFLOW)
		return FALSE;

	m_pAdapterInfo = (PIP_ADAPTER_INFO)new char[OutBufLen];
	if (m_pAdapterInfo == NULL)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return FALSE;
	}

	GetAdaptersInfo(m_pAdapterInfo, &OutBufLen);

	PIP_ADAPTER_INFO temp = m_pAdapterInfo;
	while (temp)
	{
		m_iAdapterCount++;
		temp = temp->Next;
	}
	return TRUE;
}

inline
void CAdapter::Destroy()
{
	if (m_pAdapterInfo)
	{
		delete[] m_pAdapterInfo;
		m_pAdapterInfo = NULL;
	}
	m_iAdapterCount = 0;
}

inline
PIP_ADAPTER_INFO CAdapter::GetAt(int i) const
{
	ASSERT(i < m_iAdapterCount);

	PIP_ADAPTER_INFO temp = m_pAdapterInfo;
	while (i--)
		temp = temp->Next;

	return temp;
}

inline PIP_ADAPTER_INFO CAdapter::operator [](int i) const
	{ return GetAt(i); }

inline int CAdapter::GetCount() const
	{ return m_iAdapterCount; }

#endif
