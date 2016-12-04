// strlenError.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <memory.h>
#include <windows.h>

void main()
{
	SYSTEM_INFO sSysInfo;         // Useful information about the system

	GetSystemInfo(&sSysInfo);     // Initialize the structure.
	int pageSize = sSysInfo.dwPageSize;
	char * m1, *m2;
	m1 = (char *)VirtualAlloc(0, pageSize * 2, MEM_RESERVE, PAGE_READWRITE);
	if (m1 == NULL)
	{
		printf("fail to reserve\n");
		return;
	}
	if (VirtualAlloc(m1, pageSize * 2, MEM_COMMIT, PAGE_READWRITE) == NULL)
	{
		printf("fail to commit\n");
		return;
	}
	memset(m1, 1, pageSize);
	m2 = m1 + pageSize;
	m2[0] = 0;
	strlen(m1);
	printf("first strlen ok\n");
	if (!VirtualFree(m2, pageSize, MEM_DECOMMIT))
	{
		printf("cannot free\n");	
		return;
	}
	strlen(m1);
	
}
