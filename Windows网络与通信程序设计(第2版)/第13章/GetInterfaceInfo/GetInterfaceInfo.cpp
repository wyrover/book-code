//////////////////////////////////////////////////////////////////////
// GetInterfaceInfo.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")


int main()
{
	PIP_INTERFACE_INFO pInfo = 
			(PIP_INTERFACE_INFO)::GlobalAlloc(GPTR, sizeof(IP_INTERFACE_INFO));
	ULONG ulOutBufLen = sizeof(IP_INTERFACE_INFO);

	// 如果上面申请的内存不够的话，再重新申请
	if(::GetInterfaceInfo(pInfo, &ulOutBufLen) == ERROR_INSUFFICIENT_BUFFER)
	{
		::GlobalFree(pInfo);
		pInfo = (PIP_INTERFACE_INFO)::GlobalAlloc(GPTR, ulOutBufLen);
	}

	// 再次调用GetInterfaceInfo来获取我们实际需要的数据
	if(::GetInterfaceInfo(pInfo, &ulOutBufLen) == NO_ERROR)
	{
		printf(" \tAdapter Name: %ws\n", pInfo->Adapter[0].Name);
		printf(" \tAdapter Index: %ld\n", pInfo->Adapter[0].Index);
		printf(" \tNum Adapters: %ld\n", pInfo->NumAdapters);
	}
	else
	{
		printf(" GetInterfaceInfo() failed \n");
	}

	::GlobalFree(pInfo);
	return 0;
}