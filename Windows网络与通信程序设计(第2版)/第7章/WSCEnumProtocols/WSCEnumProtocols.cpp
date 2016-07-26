///////////////////////////////////////////////////////////
// WSCEnumProtocols.cpp文件


#include <Ws2spi.h>		//  SPI函数定义在Ws2spi.h文件中
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib

LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
{
	int nError;
	DWORD dwSize = 0;
	LPWSAPROTOCOL_INFOW pProtoInfo = NULL;
	
	//  取得需要的缓冲区长度
	if(::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
	{
		if(nError != WSAENOBUFS)
			return NULL;
	}
	// 申请缓冲区，再次调用WSCEnumProtocols函数
	pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
	*lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
	return pProtoInfo;
}

void FreeProvider(LPWSAPROTOCOL_INFOW pProtoInfo)
{
	::GlobalFree(pProtoInfo);
}

void main()
{
	LPWSAPROTOCOL_INFOW pProtoInfo;
	int nProtocols;
	pProtoInfo = GetProvider(&nProtocols);

	for(int i=0; i<nProtocols; i++)
	{
		
		printf(" Protocol: %ws \n", pProtoInfo[i].szProtocol);
		printf(" CatalogEntryId: %d		ChainLen: %d \n\n", 
			pProtoInfo[i].dwCatalogEntryId, pProtoInfo[i].ProtocolChain.ChainLen);
		
	}
}