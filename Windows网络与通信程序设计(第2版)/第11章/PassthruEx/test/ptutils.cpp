//////////////////////////////////////////////
// ptutils.cpp文件

#include <windows.h>
#include <winioctl.h>
#include <ntddndis.h>
#include <stdio.h>
#include <tchar.h>

#include "../driver/IOCOMMON.h"
#include "ptutils.h"


HANDLE PtOpenControlDevice()
{
	// 打开到驱动程序所控制设备的句柄
	HANDLE hFile = ::CreateFile(
		_T("\\\\.\\PassThru"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return hFile;
}

HANDLE PtOpenAdapter(PWSTR pszAdapterName)
{
	// 打开控制设备对象句柄
	HANDLE hAdapter = PtOpenControlDevice();
	if(hAdapter == INVALID_HANDLE_VALUE)
		return INVALID_HANDLE_VALUE;

	
	// 确定适配器名称的长度
	int nBufferLength = wcslen((PWSTR)pszAdapterName) * sizeof(WCHAR);

	// 发送IOCTL_PTUSERIO_OPEN_ADAPTER控制代码，打开适配器上下文
	DWORD dwBytesReturn;
	BOOL bOK = ::DeviceIoControl(hAdapter, IOCTL_PTUSERIO_OPEN_ADAPTER, 
					pszAdapterName, nBufferLength, NULL, 0, &dwBytesReturn, NULL);
	// 检查结果
	if(!bOK)
	{
		::CloseHandle(hAdapter);
		return INVALID_HANDLE_VALUE;
	}
	return hAdapter;
}

BOOL PtAdapterRequest(HANDLE hAdapter, PPTUSERIO_OID_DATA pOidData, BOOL bQuery)
{
	if(hAdapter == INVALID_HANDLE_VALUE)
		return FALSE;
	// 发送IOCTL
	DWORD dw;
	int bRet = ::DeviceIoControl(
		hAdapter, bQuery ? IOCTL_PTUSERIO_QUERY_OID : IOCTL_PTUSERIO_SET_OID,
		pOidData, sizeof(PTUSERIO_OID_DATA) -1 + pOidData->Length,
		pOidData, sizeof(PTUSERIO_OID_DATA) -1 + pOidData->Length, &dw, NULL);

	return bRet;
}


//////////////////////////////////////

BOOL CIMAdapters::EnumAdapters(HANDLE hControlDevice)
{
	DWORD dwBufferLength = sizeof(m_buffer);
	BOOL bRet = ::DeviceIoControl(hControlDevice, IOCTL_PTUSERIO_ENUMERATE, 
		NULL, 0, m_buffer, dwBufferLength, &dwBufferLength, NULL);
	if(!bRet)
		return FALSE;
	
	// 保存适配器数量
	m_nAdapters = (ULONG)((ULONG*)m_buffer)[0];
	
	// 下面从m_buffer中获取适配器名称和符号连接名称
	// 指向设备名称
	WCHAR *pwsz = (WCHAR *)((ULONG *)m_buffer + 1);
	int i = 0;
	m_pwszVirtualName[i] = pwsz;
	while(*(pwsz++) != NULL)
	{
		while(*(pwsz++) != NULL)
		{ ; }
		
		m_pwszAdapterName[i] = pwsz;
		
		while(*(pwsz++) != NULL)
		{ ; }
		
		if(++i >= MAX_ADAPTERS)
			break;	
		
		m_pwszVirtualName[i] = pwsz;
	}
	
	return TRUE;
}

