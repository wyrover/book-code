////////////////////////////////////
// protoutils.cpp文件


#include <windows.h>
#include <winioctl.h>

#include <tchar.h>

#include <ntddndis.h>

#include <stdio.h>

#include "protoutils.h"


BOOL ProtoStartService()
{
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = NULL, hService = NULL;
	char szLinkName[] = "slNdisProt";

	// 打开SCM管理器
	hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCM != NULL)
	{
		hService = ::OpenService(hSCM, szLinkName, SERVICE_ALL_ACCESS);
		if(hService != NULL)
		{
			if(!::StartService(hService, 0, NULL))
			{
				int nError = ::GetLastError();
				if(nError == ERROR_SERVICE_ALREADY_RUNNING)
				{
					bRet = TRUE;
				}
			}
			else
			{
				bRet = TRUE;
			}
		}
	}

	if(hService != NULL)
		::CloseServiceHandle(hService);
	if(hSCM != NULL)
		::CloseServiceHandle(hSCM);

	if(bRet)
		::Sleep(50);
	return bRet;
}

void ProtoStopService()
{
	SC_HANDLE hSCM = NULL, hService = NULL;
	char szLinkName[] = "slNdisProt";

	// 打开SCM管理器
	hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCM != NULL)
	{
		hService = ::OpenService(hSCM, szLinkName, SERVICE_ALL_ACCESS);
		if(hService != NULL)
		{
				// 等待服务完全停止运行
			SERVICE_STATUS ss;
			::ControlService(hService, SERVICE_CONTROL_STOP, &ss);
		}
	}

	if(hService != NULL)
		::CloseServiceHandle(hService);
	if(hSCM != NULL)
		::CloseServiceHandle(hSCM);
}

HANDLE ProtoOpenControlDevice()
{
	// 打开驱动程序所控制设备的句柄
	HANDLE hFile = ::CreateFile(
		_T("\\\\.\\slNdisProt"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return hFile;
}

BOOL CPROTOAdapters::EnumAdapters(HANDLE hControlDevice)
{
	DWORD dwBufferLength = sizeof(m_buffer);
	BOOL bRet = ::DeviceIoControl(hControlDevice, IOCTL_ENUM_ADAPTERS, 
			NULL, 0, m_buffer, dwBufferLength, &dwBufferLength, NULL);
	if(!bRet)
		return FALSE;
	
	// 保存适配器数量
	m_nAdapters = (ULONG)((ULONG*)m_buffer)[0];
	
	// 下面从m_buffer中获取适配器名称和符号连接名称
	// 指向设备名称
	WCHAR *pwsz = (WCHAR *)((ULONG *)m_buffer + 1);
	int i = 0;
	m_pwszAdapterName[i] = pwsz;
	while(*(pwsz++) != NULL)
	{
		while(*(pwsz++) != NULL)
		{ ; }
		
		m_pwszSymbolicLink[i] = pwsz + 12; // pwsz 指向"\DosDevices\Packet_{}", 加12是为了去掉"\DosDevices\"
		
		while(*(pwsz++) != NULL)
		{ ; }
		
		if(++i >= MAX_ADAPTERS)
			break;	
		
		m_pwszAdapterName[i] = pwsz;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////

CAdapter::CAdapter()
{
	m_hAdapter = INVALID_HANDLE_VALUE;
}

CAdapter::~CAdapter()
{
	CloseAdapter();
}

BOOL CAdapter::OpenAdapter(LPCWSTR pwszSymbolicLink, BOOL bAsyn)
{
	char szFile[1024];
	wsprintf(szFile, _T("\\\\.\\%ws"), pwszSymbolicLink);
	
	// 打开到驱动程序所控制设备的句柄
	m_hAdapter = ::CreateFile(szFile,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		bAsyn ? FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED : FILE_ATTRIBUTE_NORMAL,
		NULL);
	int n = ::GetLastError();
	return m_hAdapter != INVALID_HANDLE_VALUE;
}

void CAdapter::CloseAdapter()
{
	if(m_hAdapter != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hAdapter);
		m_hAdapter = INVALID_HANDLE_VALUE;
	}
}

BOOL CAdapter::ProtoRequest(PPROTOCOL_OID_DATA pOidData, BOOL bQuery)
{
	if(m_hAdapter == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dw;
	BOOL bRet = ::DeviceIoControl(
		m_hAdapter, bQuery ? IOCTL_PROTOCOL_QUERY_OID : IOCTL_PROTOCOL_SET_OID,
		pOidData, sizeof(PROTOCOL_OID_DATA) -1 + pOidData->Length,
		pOidData, sizeof(PROTOCOL_OID_DATA) -1 + pOidData->Length, &dw, NULL);

	return bRet;
}

BOOL CAdapter::SetFilter(ULONG nFilters)
{
	PPROTOCOL_OID_DATA pOidData = (PPROTOCOL_OID_DATA)
						::GlobalAlloc(GPTR, (sizeof(PROTOCOL_OID_DATA) + sizeof(ULONG) - 1));

	pOidData->Oid = OID_GEN_CURRENT_PACKET_FILTER; 
	pOidData->Length = sizeof(ULONG);
	*((PULONG)pOidData->Data) = nFilters;

	BOOL bRet = ProtoRequest(pOidData, FALSE);

	::GlobalFree(pOidData);
	return bRet;
}

BOOL CAdapter::ResetAdapter()
{
	DWORD dw;
	BOOL bRet = ::DeviceIoControl(m_hAdapter, IOCTL_PROTOCOL_RESET, NULL, 0, NULL, 0, &dw, NULL);
	return bRet;
}

int CAdapter::RecieveData(PVOID pBuffer, int nLen, LPOVERLAPPED lpOverlapped)
{
	DWORD dwRead;
	if(::ReadFile(m_hAdapter, pBuffer, nLen, &dwRead, lpOverlapped))
		return dwRead;
	else
		return -1;
}

int CAdapter::SendData(PVOID pBuffer, int nLen, LPOVERLAPPED lpOverlapped)
{
	DWORD dwWrite;
	if(::WriteFile(m_hAdapter, pBuffer, nLen, &dwWrite, lpOverlapped))
		return dwWrite;
	else
		return -1;
}