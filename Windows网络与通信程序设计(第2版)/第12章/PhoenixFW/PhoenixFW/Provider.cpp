/////////////////////////////////////////////////
// 金羽(Phoenix)个人防火墙 V1.1



#include "stdafx.h"

#include <Ws2spi.h>
#include <Sporder.h>


// 要安装的LSP的硬编码，在移除的时候还要使用它
GUID  ProviderGuid = {0xd3c21122, 0x85e1, 0x48f3, 
							{0x9a,0xb6,0x23,0xd9,0x0c,0x73,0x07,0xef}};



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



BOOL InstallProvider(WCHAR *pwszPathName)
{
	WCHAR wszLSPName[] = L"PhoenixLSP";
	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols;
	WSAPROTOCOL_INFOW OriginalProtocolInfo[3];
	DWORD			 dwOrigCatalogId[3];
	int nArrayCount = 0;

	DWORD dwLayeredCatalogId;		// 我们分层协议的目录ID号

	int nError;
	
		// 找到我们的下层协议，将信息放入数组中
	// 枚举所有服务程序提供者
	pProtoInfo = GetProvider(&nProtocols);
	BOOL bFindUdp = FALSE;
	BOOL bFindTcp = FALSE;
	BOOL bFindRaw = FALSE;
	for(int i=0; i<nProtocols; i++)
	{
		if(pProtoInfo[i].iAddressFamily == AF_INET)
		{
		if(!bFindUdp && pProtoInfo[i].iProtocol == IPPROTO_UDP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = 
					OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;

				bFindUdp = TRUE;
			}

		if(!bFindTcp && pProtoInfo[i].iProtocol == IPPROTO_TCP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = 
					OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;

				bFindTcp = TRUE;
			} 
		if(!bFindRaw && pProtoInfo[i].iProtocol == IPPROTO_IP)
			{
				memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
				OriginalProtocolInfo[nArrayCount].dwServiceFlags1 = 
					OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES); 
				
				dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;

				bFindRaw = TRUE;
			}
		}
	}  

		// 安装我们的分层协议，获取一个dwLayeredCatalogId
	// 随便找一个下层协议的结构复制过来即可
	WSAPROTOCOL_INFOW LayeredProtocolInfo;
	memcpy(&LayeredProtocolInfo, &OriginalProtocolInfo[0], sizeof(WSAPROTOCOL_INFOW));
	// 修改协议名称，类型，设置PFL_HIDDEN标志
	wcscpy(LayeredProtocolInfo.szProtocol, wszLSPName);
	LayeredProtocolInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL; // 0;
	LayeredProtocolInfo.dwProviderFlags |= PFL_HIDDEN;
	// 安装
	if(::WSCInstallProvider(&ProviderGuid, 
					pwszPathName, &LayeredProtocolInfo, 1, &nError) == SOCKET_ERROR)
	{
		return FALSE;
	}
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
	for(i=0; i<nArrayCount; i++)
	{
		swprintf(wszChainName, L"%ws over %ws", wszLSPName, OriginalProtocolInfo[i].szProtocol);
		wcscpy(OriginalProtocolInfo[i].szProtocol, wszChainName);
		if(OriginalProtocolInfo[i].ProtocolChain.ChainLen == 1)
		{
			OriginalProtocolInfo[i].ProtocolChain.ChainEntries[1] = dwOrigCatalogId[i];
		}
		else
		{
			for(int j = OriginalProtocolInfo[i].ProtocolChain.ChainLen; j>0; j--)
			{
				OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j] 
									= OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j-1];
			}
		}
		OriginalProtocolInfo[i].ProtocolChain.ChainLen ++;
		OriginalProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogId;	
	}
	// 获取一个Guid，安装之
	GUID ProviderChainGuid;
	if(::UuidCreate(&ProviderChainGuid) == RPC_S_OK)
	{
		if(::WSCInstallProvider(&ProviderChainGuid, 
					pwszPathName, OriginalProtocolInfo, nArrayCount, &nError) == SOCKET_ERROR)
		{
			return FALSE;	
		}
	}
	else
		return FALSE;

			// 将我们的协议提前，重新排序Winsock目录
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
	if(nError = ::WSCWriteProviderOrder(dwIds, nIndex) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	FreeProvider(pProtoInfo);

	return TRUE;
}

BOOL RemoveProvider()
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

	return TRUE;
}













/////////////////////////////////////////////////////////////////////
void RemoveAllLayeredEntries()
{
	 BOOL    bLayer;
    int     ErrorCode,
            i;

	int TotalProtocols;
	
	LPWSAPROTOCOL_INFOW ProtocolInfo;

    while (1)
    {
        bLayer = FALSE;
        ProtocolInfo = GetProvider(&TotalProtocols);
        if (!ProtocolInfo)
        {
            printf("Unable to enumerate Winsock catalog!\n");
            return;
        }
        for(i=0; i < TotalProtocols ;i++)
        {
            if (ProtocolInfo[i].ProtocolChain.ChainLen != BASE_PROTOCOL)
            {
                bLayer = TRUE;
                printf("Removing '%S'\n", ProtocolInfo[i].szProtocol);
                if (WSCDeinstallProvider(&ProtocolInfo[i].ProviderId, &ErrorCode) == SOCKET_ERROR)
                {
                    printf("Failed to remove [%s]: Error %d\n", ProtocolInfo[i].szProtocol, ErrorCode);
                }
                break;
            }
        }
        FreeProvider(ProtocolInfo);
        if (bLayer == FALSE)
        {
            break;
        }

    }
}