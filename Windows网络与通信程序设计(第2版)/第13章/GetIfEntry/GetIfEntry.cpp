//////////////////////////////////////////////////////////////////////
// GetIfEntry.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")


int main()
{
	PMIB_IFTABLE pIfTable;
	PMIB_IFROW pMibIfRow;
	DWORD dwSize = 0;

	// 为输出缓冲区申请内存
	pIfTable = (PMIB_IFTABLE)::GlobalAlloc(GPTR, sizeof(MIB_IFTABLE));
	pMibIfRow = (PMIB_IFROW)::GlobalAlloc(GPTR, sizeof(MIB_IFROW));

	// 在调用GetIfEntry之前，我们调用GetIfTable以确保入口存在

	// 获取所需内存的大小
	if(::GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
	{
		::GlobalFree(pIfTable);
		pIfTable = (PMIB_IFTABLE)::GlobalAlloc(GPTR, dwSize);
	}

	// 再次调用GetIfTable来获取我们想要的实际数据
	if(::GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR)
	{
		if(pIfTable->dwNumEntries > 0)
		{
			pMibIfRow->dwIndex = 1;
			// 获取第一个接口信息
			if(::GetIfEntry(pMibIfRow) == NO_ERROR)
			{
				printf(" Description: %s\n", pMibIfRow->bDescr);
			}
			else
			{
				printf(" GetIfEntry failed.\n");
			}
		}
	}
	else
	{
		printf(" GetIfTable failed.\n");
	}


	::GlobalFree(pIfTable);
	return 0;
}