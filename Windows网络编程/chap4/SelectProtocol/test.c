//
// TEST.C -- Test the SelectProtocols() function
//

#include <stdio.h>
#include <winsock2.h>

int SelectProtocols(
					DWORD dwSetFlags,
					DWORD dwNotSetFlags,
					LPWSAPROTOCOL_INFO lpProtocolBuffer,
					LPDWORD lpdwBufferLength
					);

void TestSelectProtocols(void);

void main(void)
{
	WORD wVersionRequested = MAKEWORD(2,2);
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet)
	{
		fprintf(stderr,"\nWSAStartup() error (%d)\n", nRet);
		return;
	}

	TestSelectProtocols();

	WSACleanup();
}

void TestSelectProtocols(void)
{
	LPBYTE				pBuf;
	LPWSAPROTOCOL_INFO	pInfo;
	DWORD				dwLen;
	int nCount;
	int nRet;

	//
	// Determine needed buffer size
	//
	dwLen = 0;
	nRet = WSAEnumProtocols(NULL, NULL, &dwLen);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAENOBUFS)
		{
			fprintf(stderr,"\nEnumProtocols(): %d\n",
						WSAGetLastError());
			return;
		}
	}

	//
	// Allocate the buffer
	//
	pBuf = malloc(dwLen);
	if (pBuf == NULL)
	{
		fprintf(stderr,"\nCouldn't allocate buffer\n");
	}

	printf("\nRequesting only connection-oriented, reliable protocols\n");
	//
	// Request only connection-oriented, reliable protocols
	//
	#define SETFLAGS XP1_GUARANTEED_DELIVERY|XP1_GUARANTEED_ORDER
	#define NOTSETFLAGS XP1_CONNECTIONLESS

	nRet = SelectProtocols(SETFLAGS,
						   NOTSETFLAGS,
						   (LPWSAPROTOCOL_INFO)pBuf,
						   &dwLen);
	if (nRet == SOCKET_ERROR)
	{
		fprintf(stderr,"\nSelectProtocols(): %d\n",
					WSAGetLastError());
		return;
	}

	//
	// Print the returned protocol names and
	// socket() parameters
	//
	pInfo = (LPWSAPROTOCOL_INFO)pBuf;	
	for(nCount = 0; nCount < nRet; nCount++)
	{
		printf("\n----- Protocol %d: %s -----", nCount, 
						pInfo->szProtocol);
		printf("\nVersion: %d", pInfo->iVersion);
		printf("\nsocket(%d, %d, %d)\n",
				pInfo->iAddressFamily,
				pInfo->iSocketType,
				pInfo->iProtocol);
		pInfo++;
	}
	free(pBuf);
}

