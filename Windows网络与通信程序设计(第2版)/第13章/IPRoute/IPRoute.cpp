//////////////////////////////////////////////////////////////////////
// IPRoute.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")


PMIB_IPFORWARDTABLE MyGetIpForwardTable(BOOL bOrder);
void MyFreeIpForwardTable(PMIB_IPFORWARDTABLE pIpRouteTab);
void PrintIpForwardTable();



int main()
{
	PrintIpForwardTable();

	return 0;
}

void PrintIpForwardTable()
{
	PMIB_IPFORWARDTABLE pIpRouteTable = MyGetIpForwardTable(TRUE);
	if(pIpRouteTable != NULL)
	{
		DWORD i, dwCurrIndex;
		struct in_addr inadDest;
		struct in_addr inadMask;
		struct in_addr inadGateway;  
		PMIB_IPADDRTABLE pIpAddrTable = NULL;
		
		char szDestIp[128];
		char szMaskIp[128];
		char szGatewayIp[128];

		printf("Active Routes:\n\n");
		
		printf("  Network Address          Netmask  Gateway Address        Interface  Metric\n");
		for (i = 0; i < pIpRouteTable->dwNumEntries; i++)
		{
			dwCurrIndex = pIpRouteTable->table[i].dwForwardIfIndex;
	
			// 目的地址
			inadDest.s_addr = pIpRouteTable->table[i].dwForwardDest;
			// 子网掩码
			inadMask.s_addr = pIpRouteTable->table[i].dwForwardMask;
			// 网关地址
			inadGateway.s_addr = pIpRouteTable->table[i].dwForwardNextHop;
			
			strcpy(szDestIp, inet_ntoa(inadDest));
			strcpy(szMaskIp, inet_ntoa(inadMask));
			strcpy(szGatewayIp, inet_ntoa(inadGateway));
			printf("  %15s %16s %16s %16d %7d\n", 
                szDestIp, 
                szMaskIp, 
                szGatewayIp, 
                pIpRouteTable->table[i].dwForwardIfIndex,	// 可以在此调用GetIpAddrTable获取索引对应的IP地址
                pIpRouteTable->table[i].dwForwardMetric1);
		}
		MyFreeIpForwardTable(pIpRouteTable);
	}
}

PMIB_IPFORWARDTABLE MyGetIpForwardTable(BOOL bOrder)
{
	PMIB_IPFORWARDTABLE pIpRouteTab = NULL;
    DWORD dwActualSize = 0;
    
    // 查询所需缓冲区的大小
    if(::GetIpForwardTable(pIpRouteTab, &dwActualSize, bOrder) == ERROR_INSUFFICIENT_BUFFER)
	{
        // 为MIB_IPFORWARDTABLE结构申请内存
        pIpRouteTab = (PMIB_IPFORWARDTABLE)::GlobalAlloc(GPTR, dwActualSize);
        // 获取路由表
		if(::GetIpForwardTable(pIpRouteTab, &dwActualSize, bOrder) == NO_ERROR)
			return pIpRouteTab;
		::GlobalFree(pIpRouteTab);
    }
	return NULL;
}

void MyFreeIpForwardTable(PMIB_IPFORWARDTABLE pIpRouteTab)
{
	if(pIpRouteTab != NULL)
		::GlobalFree(pIpRouteTab);
}