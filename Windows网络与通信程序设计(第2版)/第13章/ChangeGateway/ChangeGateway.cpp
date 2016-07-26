//////////////////////////////////////////////////////////////////////
// ChangeGateway.cpp文件


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
	// 新网关地址
	DWORD dwNewGateway = ::inet_addr("192.168.0.1");

	// 在表中查找我们想要的入口。默认网关的目的地址为0.0.0.0
	PMIB_IPFORWARDTABLE pIpRouteTable = MyGetIpForwardTable(TRUE);
	PMIB_IPFORWARDROW pRow = NULL;
	if(pIpRouteTable != NULL)
	{
		for(DWORD i=0; i<pIpRouteTable->dwNumEntries; i++)
		{
			if(pIpRouteTable->table[i].dwForwardDest == 0)	// 找到了默认网关
			{
				// 申请内存来保存这个入口。
				// 这比自己填充MIB_IPFORWARDROW结构简单许多，我们仅需要改变网关地址
				if(pRow == NULL)
				{
					pRow = (PMIB_IPFORWARDROW)::GlobalAlloc(GPTR, sizeof(MIB_IPFORWARDROW));
					memcpy(pRow, &pIpRouteTable->table[i], sizeof(MIB_IPFORWARDROW));
				}

				// 删除旧的默认网关入口
				if(::DeleteIpForwardEntry(&pIpRouteTable->table[i]) != ERROR_SUCCESS)
				{
					printf("Could not delete old gateway \n");
					exit(1);
				}
			}
		}
		MyFreeIpForwardTable(pIpRouteTable);
	}

	if(pRow != NULL)
	{
		// 设置dwForwardNextHop域为我们的新网关，所有其它的路由属性将和先前的相同
		pRow->dwForwardNextHop = dwNewGateway;

		// 为默认网关创建新的路由入口
		if(::SetIpForwardEntry(pRow) == NO_ERROR)
			printf(" Gateway changed successfully \n");
		else
			printf(" SetIpForwardEntry() failed \n");

		::GlobalFree(pRow);
	}

	return 0;
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