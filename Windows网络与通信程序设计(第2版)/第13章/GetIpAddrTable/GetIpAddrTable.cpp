//////////////////////////////////////////////////////////////////////
// GetIpAddrTable.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")


int main()
{
		// 首先调用GetIpAddrTable函数获取一个适配器
	PMIB_IPADDRTABLE pIPAddrTable;
	DWORD dwSize = 0;

	pIPAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));

	// 获取所需的内存
	if(::GetIpAddrTable(pIPAddrTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
	{
		::GlobalFree(pIPAddrTable);
		pIPAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, dwSize);
	}

	// 再次调用GetIpAddrTable获取实际我们想要的数据
	if(::GetIpAddrTable(pIPAddrTable, &dwSize, FALSE) == NO_ERROR)
	{
		// 打印出适配器信息
		printf(" Address: %ld\n", pIPAddrTable->table[0].dwAddr);
		printf(" Mask:    %ld\n", pIPAddrTable->table[0].dwMask);
		printf(" Index:   %ld\n", pIPAddrTable->table[0].dwIndex);
		printf(" BCast:   %ld\n", pIPAddrTable->table[0].dwBCastAddr);
		printf(" Reasm:   %ld\n", pIPAddrTable->table[0].dwReasmSize);
	}	
	else
	{
		printf(" GetIpAddrTable() failed \n");
	}
	
	::GlobalFree(pIPAddrTable);
	
	// 我们将要添加的IP和mast
	UINT iaIPAddress;
	UINT imIPMask;
	
	iaIPAddress = inet_addr("192.168.0.27");
	imIPMask = inet_addr("255.255.255.0");
	
	// 返回的句柄
	ULONG NTEContext = 0;
	ULONG NTEInstance = 0;
	
	// 向第一个适配器添加IP地址
	DWORD dwRet;
	dwRet = ::AddIPAddress(iaIPAddress, imIPMask, 
				pIPAddrTable->table[0].dwIndex, &NTEContext, &NTEInstance);
	if(dwRet == NO_ERROR) 
	{
		printf(" IP address added.\n");
	}
	else 
	{
		printf(" AddIPAddress failed. \n");
		LPVOID lpMsgBuf;
		// 调用失败，打印出为什么失败
		if (::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwRet,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL )) 
		{
			printf(" Error: %s ", lpMsgBuf);
		}
		::LocalFree(lpMsgBuf);
	}
	
	// 删除上面在第一个适配器上添加的IP地址
	dwRet = ::DeleteIPAddress(NTEContext);
	if(dwRet == NO_ERROR) 
	{
		printf(" IP Address Deleted.\n");
	}
	else 
	{		
		printf(" DeleteIPAddress failed.\n");
	}
	
	return 0;
}