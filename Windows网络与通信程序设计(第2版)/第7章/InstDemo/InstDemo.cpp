////////////////////////////////////////////////////////
// InstDemo.cpp

#include <Ws2spi.h>
#include <Sporder.h>				// 定义了WSCWriteProviderOrder函数

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")	// 实现了UuidCreate函数


// 要安装的LSP的硬编码，在移除的时候还要使用它
GUID  ProviderGuid = {0xd3c21122, 0x85e1, 0x48f3, {0x9a,0xb6,0x23,0xd9,0x0c,0x73,0x07,0xef}};


LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
{
	DWORD dwSize = 0;
	int nError;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	
	// 取得需要的长度
	if(::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
	{
		if(nError != WSAENOBUFS)
			return NULL;
	}
	
	pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
	*lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
	return pProtoInfo;
}

void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
{
	::GlobalFree(pProtoInfo);
}


// 将LSP安装到UDP协议提供者之上
int InstallProvider(WCHAR *wszDllPath)
{
	WCHAR wszLSPName[] = L"TinyLSP";	// 我们的LSP的名称
	int nError = NO_ERROR;

	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols;
	WSAPROTOCOL_INFOW UDPLayeredInfo, UDPChainInfo; // 我们要安装的UDP分层协议和协议链
	DWORD dwUdpOrigCatalogId, dwLayeredCatalogId;

		// 在Winsock目录中找到原来的UDP协议服务提供者，我们的LSP要安装在它之上
	// 枚举所有服务程序提供者
	pProtoInfo = GetProvider(&nProtocols);
	for(int i=0; i<nProtocols; i++)
	{
		if(pProtoInfo[i].iAddressFamily == AF_INET && pProtoInfo[i].iProtocol == IPPROTO_UDP)
		{
			memcpy(&UDPChainInfo, &pProtoInfo[i], sizeof(UDPLayeredInfo));
			// 
			UDPChainInfo.dwServiceFlags1 = UDPChainInfo.dwServiceFlags1 & ~XP1_IFS_HANDLES;  
			// 保存原来的入口ID
			dwUdpOrigCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}  

		// 首先安装分层协议，获取一个Winsock库安排的目录ID号，即dwLayeredCatalogId
	// 直接使用下层协议的WSAPROTOCOL_INFOW结构即可
	memcpy(&UDPLayeredInfo, &UDPChainInfo, sizeof(UDPLayeredInfo));
	// 修改协议名称，类型，设置PFL_HIDDEN标志
	wcscpy(UDPLayeredInfo.szProtocol, wszLSPName);
	UDPLayeredInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;		// LAYERED_PROTOCOL即0
	UDPLayeredInfo.dwProviderFlags |= PFL_HIDDEN;
	// 安装
	if(::WSCInstallProvider(&ProviderGuid, 
					wszDllPath, &UDPLayeredInfo, 1, &nError) == SOCKET_ERROR)
		return nError;
	// 重新枚举协议，获取分层协议的目录ID号
	FreeProvider(pProtoInfo);
	pProtoInfo = GetProvider(&nProtocols);
	for(i=0; i<nProtocols; i++)
	{
		if(memcmp(&pProtoInfo[i].ProviderId, &ProviderGuid, sizeof(ProviderGuid)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}

		// 安装协议链
	// 修改协议名称，类型
	WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
	swprintf(wszChainName, L"%ws over %ws", wszLSPName, UDPChainInfo.szProtocol);
	wcscpy(UDPChainInfo.szProtocol, wszChainName);
	if(UDPChainInfo.ProtocolChain.ChainLen == 1)
	{
		UDPChainInfo.ProtocolChain.ChainEntries[1] = dwUdpOrigCatalogId;
	}
	else
	{
		for(i=UDPChainInfo.ProtocolChain.ChainLen; i>0 ; i--)
		{
			UDPChainInfo.ProtocolChain.ChainEntries[i] = UDPChainInfo.ProtocolChain.ChainEntries[i-1];
		}
	}
	UDPChainInfo.ProtocolChain.ChainLen ++;
	// 将我们的分层协议置于此协议链的顶层
	UDPChainInfo.ProtocolChain.ChainEntries[0] = dwLayeredCatalogId; 
	// 获取一个Guid，安装之
	GUID ProviderChainGuid;
	if(::UuidCreate(&ProviderChainGuid) == RPC_S_OK)
	{
		if(::WSCInstallProvider(&ProviderChainGuid, 
					wszDllPath, &UDPChainInfo, 1, &nError) == SOCKET_ERROR)
					return nError;
	}
	else
		return GetLastError();



		// 重新排序Winsock目录，将我们的协议链提前
	// 重新枚举安装的协议
	FreeProvider(pProtoInfo);
	pProtoInfo = GetProvider(&nProtocols);

	DWORD dwIds[20];
	int nIndex = 0;
	// 添加我们的协议链
	for(i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen > 1) &&
					(pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
			dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
	}
	// 添加其它协议
	for(i=0; i<nProtocols; i++)
	{
		if((pProtoInfo[i].ProtocolChain.ChainLen <= 1) ||
				(pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId))
			dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
	}
	// 重新排序Winsock目录
	nError = ::WSCWriteProviderOrder(dwIds, nIndex);

	FreeProvider(pProtoInfo);
	return nError;
}

void RemoveProvider()
{	
	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols;
	DWORD dwLayeredCatalogId;

	// 根据Guid取得分层协议的目录ID号
	pProtoInfo = GetProvider(&nProtocols);
	int nError;
	for(int i=0; i<nProtocols; i++)
	{
		if(memcmp(&ProviderGuid, &pProtoInfo[i].ProviderId, sizeof(ProviderGuid)) == 0)
		{
			dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
			break;
		}
	}

	if(i < nProtocols)
	{
		// 移除协议链
		for(i=0; i<nProtocols; i++)
		{
			if((pProtoInfo[i].ProtocolChain.ChainLen > 1) &&
					(pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
			{
				::WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &nError);
			}
		}
		// 移除分层协议
		::WSCDeinstallProvider(&ProviderGuid, &nError);
	}
}



////////////////////////////////////////////////////

int binstall = 0;
void main()
{
	if(binstall)
	{
		if(InstallProvider(L"lsp.dll") == ERROR_SUCCESS)
		{
			printf(" Install successully \n");
		}
		else
		{
			printf(" Install failed \n");
		}
	}
	else
		RemoveProvider();
}