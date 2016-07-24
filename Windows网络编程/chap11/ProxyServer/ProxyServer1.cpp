// ProxyServer1.cpp: implementation of the CProxyServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProxyServer.h"
#include "ProxyServer1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProxyServer::CProxyServer()
{

}

CProxyServer::~CProxyServer()
{

}


UINT CProxyServer::ListenThread(LPVOID lpVoid)
{
    int iRet, addrLen;
	sockaddr_in addr, addrAccept;
	SOCKET sckListen, sckAccept;
	char pszMsg[512];
	int nErrCount;
   
	CProxyServer* pServer = (CProxyServer*)lpVoid;
    pServer->OUTPUT(pServer,"going to create socket.\r\n");

	sckListen = socket(AF_INET, SOCK_STREAM, 0);
	if(sckListen == INVALID_SOCKET)
	{
		sprintf(pszMsg, "INVALID_SOCKET, WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		return 0;
	}
	pServer->OUTPUT(pServer,"Listening socket created successfully.\r\n");

	iRet = gethostname(pszMsg, 256);
	if(iRet)
	{
		sprintf(pszMsg, "Error occur when gethostname(), WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		return 0;
	}

	hostent* pEnt = gethostbyname(pszMsg);
	if(!pEnt)
	{
		sprintf(pszMsg, "Error occur when gethostbyname(), WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		return 0;
	}
    
	memcpy(&(addr.sin_addr), pEnt->h_addr, pEnt->h_length);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(pServer->m_Info.nPort);
	
	pServer->m_Info.strLocalAddr.Format("%s", inet_ntoa(addr.sin_addr));

	iRet = bind(sckListen, (const sockaddr*)&addr, sizeof(addr));
	if(iRet==SOCKET_ERROR)
	{
		sprintf(pszMsg, "SOCKET_ERROR, WSAGetLastError()=%d", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		return 0;
	}

	nErrCount = 0;
	pServer->OUTPUT(pServer,"Listening...\r\n");
	iRet = listen(sckListen, SOMAXCONN);
	if(iRet==SOCKET_ERROR)
	{
		sprintf(pszMsg, "INVALID_SOCKET, WSAGetLastError()=%d", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		nErrCount++;
		if(nErrCount>=10)
		{
			sprintf(pszMsg, "nErrCount>=10, listening thread terminated.\r\n");
			return 0;
		}
	}
	nErrCount = 0;

	while(1)
	{
		addrLen = sizeof(addrAccept);
		sckAccept = accept(sckListen, (struct sockaddr*)&addrAccept, &addrLen);
		if(sckAccept==INVALID_SOCKET)
		{
			sprintf(pszMsg, "accept()=SOCKET_ERROR, WSAGetLastError()=%d", WSAGetLastError());
			pServer->OUTPUT(pServer,pszMsg);
			return 0;
		}
		pServer->OUTPUT(pServer,"One client accepted successfully.\r\n");

		pServer->pWorkParam = new WORKPARAM;
		pServer->pWorkParam->sckClient = sckAccept;
		pServer->pWorkParam->addr = addrAccept;

		AfxBeginThread(WorkingThread, (LPVOID)pServer, THREAD_PRIORITY_BELOW_NORMAL);
	}

	return 0;
}

void CProxyServer::OUTPUT(CProxyServer* pServer,LPCTSTR pszMsg)
{
   SendMessage(pServer->m_Info.hWnd, AGM_OUTPUT, 0, (LPARAM)pszMsg);

}

UINT CProxyServer::WorkingThread(LPVOID lpVoid)
{
    int iRet, i;
	char content[512];
	char buf[BUFFER_SIZE];
	char pszMsg[1024];
	char pszCode[256];
	char pszLine[512];
	char *pEnd;
	CProxyServer* pServer = (CProxyServer*)lpVoid;
    WORKPARAM * pThreadParam = pServer->pWorkParam;
	DWORD dwRet, dwTotal, dwSize;
	sockaddr_in addrProxy2nd;
	SOCKET sckClient;
	REQUESTHEADER requestHeader;
	HTTPHEADER httpHeader;
	FILE* pFileContent;

	dwRet = WaitForSingleObject(pServer->m_hConnectionCount, INFINITE);
	if(pServer->m_nConnectionCount >= 80)
	{
		ReleaseSemaphore(pServer->m_hConnectionCount, 1, NULL);
		pServer->OUTPUT(pServer,"Too many connections, limit reached\r\n");
		strcpy(buf, "HTTP/1.1 431 Too many connections, limit reached\r\n"); send(pThreadParam->sckClient , buf, strlen(buf), 0);
		strcpy(buf, "Proxy-agent: My-PersonalProxy/1.0\r\n");
		send(pThreadParam->sckClient, buf, strlen(buf), 0);
		strcpy(buf, "Content-type: text/html\r\n"); 
		send(pThreadParam->sckClient, buf, strlen(buf), 0);

		pFileContent = fopen("c:\\ProxyServer\\authorizationrequired.html", "rb");
		dwSize = _filelength(_fileno(pFileContent));
		sprintf(buf, "Content-length: %d\r\n", dwSize); 
		send(pThreadParam->sckClient, buf, strlen(buf), 0);
		strcpy(buf, "\r\n"); 
		send(pThreadParam->sckClient, buf, strlen(buf), 0);

		while(!feof(pFileContent))
		{
			dwSize = fread(buf, 1, BUFFER_SIZE, pFileContent);
			send(pThreadParam->sckClient, buf, dwSize, 0);
		}
		closesocket(pThreadParam->sckClient);
		fclose(pFileContent);
		delete pThreadParam;
		return 0;
	}
	pServer->m_nConnectionCount++;
	ReleaseSemaphore(pServer->m_hConnectionCount, 1, NULL);

	while(1)
	{
		iRet = pServer->RecvRequest(pServer,pThreadParam->sckClient, buf, BUFFER_SIZE-1, &requestHeader);
		
		
	 	if(iRet==SOCKET_ERROR)
		{
			sprintf(pszMsg, "RecvRequest()=SOCKET_ERROR, WSAGetLastError()=%d", WSAGetLastError());
			pServer->OUTPUT(pServer,pszMsg);
			WaitForSingleObject(pServer->m_hConnectionCount, INFINITE);
			pServer->m_nConnectionCount--;
			ReleaseSemaphore(pServer->m_hConnectionCount, 1, NULL);
			delete pThreadParam;
			return 0;
		}
		dwTotal = iRet;

		for(i=0; i<pServer->m_nRefused; i++)
		{
			if(pServer->m_ulIpRefused[i]==pThreadParam->addr.sin_addr.S_un.S_addr)
			{
				pServer->OUTPUT(pServer,"refused ip:");
				sprintf(content, "\t%d.%d.%d.%d\r\n",
					pThreadParam->addr.sin_addr.S_un.S_un_b.s_b1,
					pThreadParam->addr.sin_addr.S_un.S_un_b.s_b2,
					pThreadParam->addr.sin_addr.S_un.S_un_b.s_b3,
					pThreadParam->addr.sin_addr.S_un.S_un_b.s_b4);
				pServer->OUTPUT(pServer,content);
				strcpy(buf, "HTTP/1.1 432 Ip refused\r\n"); 
				send(pThreadParam->sckClient , buf, strlen(buf), 0);
				strcpy(buf, "Proxy-agent: Ag-PersonalProxy/1.0\r\n");
				send(pThreadParam->sckClient, buf, strlen(buf), 0);
				strcpy(buf, "Content-type: text/html\r\n"); 
				send(pThreadParam->sckClient, buf, strlen(buf), 0);
				pFileContent = fopen("c:\\ProxyServer\\notification.html", "rb");
				dwSize = _filelength(_fileno(pFileContent));
				sprintf(buf, "Content-length: %d\r\n", dwSize); 
				send(pThreadParam->sckClient, buf, strlen(buf), 0);
				strcpy(buf, "\r\n");
				send(pThreadParam->sckClient, buf, strlen(buf), 0);

					dwSize = fread(buf, 1, BUFFER_SIZE, pFileContent);
					buf[dwSize] = NULL;
					sprintf(content, buf,
						pThreadParam->addr.sin_addr.S_un.S_un_b.s_b1,
						pThreadParam->addr.sin_addr.S_un.S_un_b.s_b2,
						pThreadParam->addr.sin_addr.S_un.S_un_b.s_b3,
						pThreadParam->addr.sin_addr.S_un.S_un_b.s_b4);
					send(pThreadParam->sckClient, content, strlen(content), 0);
				closesocket(pThreadParam->sckClient);
				fclose(pFileContent);
				goto _ThreadExit;
			}
		}
		if(requestHeader.dwFlag & REQUEST_UNAUTHORIZED)
		{
			pServer->OUTPUT(pServer,"HTTP/1.1 407 Proxy authorization required agproxy.");
			sprintf(content, "\t%s\t%d.%d.%d.%d\r\n",
				requestHeader.pszUserPass,
				pThreadParam->addr.sin_addr.S_un.S_un_b.s_b1,
				pThreadParam->addr.sin_addr.S_un.S_un_b.s_b2,
				pThreadParam->addr.sin_addr.S_un.S_un_b.s_b3,
				pThreadParam->addr.sin_addr.S_un.S_un_b.s_b4);
			pServer->OUTPUT(pServer,content);
			//HTTP/1.0 407 Proxy authorization required
			strcpy(buf, "HTTP/1.1 407 Proxy authorization required\r\n"); send(pThreadParam->sckClient , buf, strlen(buf), 0);
			strcpy(buf, "Proxy-agent: Ag-PersonalProxy/1.0\r\n"); 
			send(pThreadParam->sckClient, buf, strlen(buf), 0);
			strcpy(buf, "Content-type: image/jpeg\r\n");
			send(pThreadParam->sckClient, buf, strlen(buf), 0);
			strcpy(buf, "Proxy-authenticate: basic realm=\"JutAg\"\r\n"); 
			send(pThreadParam->sckClient, buf, strlen(buf), 0);
			pFileContent = fopen("c:\\ProxyServer\\Authorization.jpg", "rb");
			dwSize = _filelength(_fileno(pFileContent));
			sprintf(buf, "Content-length: %d\r\n", dwSize); 
			send(pThreadParam->sckClient, buf, strlen(buf), 0);
			strcpy(buf, "\r\n");
			send(pThreadParam->sckClient, buf, strlen(buf), 0);
			
			while(!feof(pFileContent))
			{
				dwSize = fread(buf, 1, BUFFER_SIZE, pFileContent);
				send(pThreadParam->sckClient, buf, dwSize, 0);
			}
				fclose(pFileContent);
			continue;
		}
		break;
	}

	if(strstr(buf, "POST"))
	{
		pServer->OUTPUT(pServer,buf);
		pServer->OUTPUT(pServer,"\r\n");
	}
	else
	{
		strncpy(content, buf, strstr(buf, "\r\n")-buf+2);
		content[strstr(buf, "\r\n")-buf] = NULL;
		pServer->OUTPUT(pServer,content);
	}
	sprintf(content, "\t%d.%d.%d.%d\r\n",
		pThreadParam->addr.sin_addr.S_un.S_un_b.s_b1,
		pThreadParam->addr.sin_addr.S_un.S_un_b.s_b2,
		pThreadParam->addr.sin_addr.S_un.S_un_b.s_b3,
		pThreadParam->addr.sin_addr.S_un.S_un_b.s_b4);
	pServer->OUTPUT(pServer,content);

	if(requestHeader.dwFlag & VIRTUAL_RESOURCE)
	{
		pServer->HttpService(pServer,requestHeader.pszUrl, pThreadParam->sckClient, buf, &requestHeader);
		goto _ThreadExit;
	}

	pServer->OUTPUT(pServer,"Connecting to the second proxy.\r\n");
	addrProxy2nd.sin_family = AF_INET;
	addrProxy2nd.sin_port = htons(pServer->m_Info.nPort2nd);
	addrProxy2nd.sin_addr.s_addr = inet_addr(pServer->m_Info.strProxyAddr);
 
	sckClient = socket(AF_INET, SOCK_STREAM, 0);
	if(sckClient==INVALID_SOCKET)
	{
		closesocket(pThreadParam->sckClient);
		closesocket(sckClient);
		sprintf(pszMsg, "socket()=INVALID_SOCKET, WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		goto _ThreadExit1;
	}
	iRet = connect(sckClient, (const sockaddr*)&addrProxy2nd, sizeof(addrProxy2nd));
	if(iRet==SOCKET_ERROR)
	{
		closesocket(pThreadParam->sckClient);
		closesocket(sckClient);
		sprintf(pszMsg, "connect()=SOCKET_ERROR, WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
		pServer->OUTPUT(pServer,pszMsg);
		goto _ThreadExit1;
	}
	
	// send the data before	Proxy-Authorization:
	send(sckClient, buf, requestHeader.pszAuthBegin-buf, 0);
	strncpy(pszMsg, buf, requestHeader.pszAuthBegin-buf);
	pszMsg[requestHeader.pszAuthBegin-buf] = NULL;
//	TRACE(pszMsg);
	// send Authorization information to the second proxy
	sprintf(pszMsg, "%s:%s", pServer->m_Info.strUser, pServer->m_Info.strPasswd);
	pServer->Base64(pServer,pszMsg, pszCode);
	sprintf(content, "Proxy-Authorization: Basic %s==\r\n", pszCode);
	send(sckClient, content, strlen(content), 0);
//	TRACE(content);
//	send(sckClient, requestHeader.pszAuthBegin, requestHeader.pszAuthEnd-requestHeader.pszAuthBegin, 0);
	strncpy(pszMsg, requestHeader.pszAuthBegin, requestHeader.pszAuthEnd-requestHeader.pszAuthBegin);
	pszMsg[requestHeader.pszAuthBegin, requestHeader.pszAuthEnd-requestHeader.pszAuthBegin] = NULL;
//	TRACE(pszMsg);
	// send the rest information
	send(sckClient, requestHeader.pszAuthEnd, strlen(requestHeader.pszAuthEnd), 0);
	iRet = 1;
	while(dwTotal<requestHeader.dwContSize && iRet)
	{
		iRet = recv(pThreadParam->sckClient, buf, BUFFER_SIZE, 0);
		buf[iRet] = NULL;
		if(iRet==SOCKET_ERROR)
		{
			sprintf(pszMsg, "while(nTotal<requestHeader.dwContSize && iRet) recv()=SOCKET_ERROR, WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
			pServer->OUTPUT(pServer,pszMsg);
			goto _ThreadExit;
		}
		if(iRet)
		{
			pServer->OUTPUT(pServer,"_DEBUG_PRE_\t");
			pServer->OUTPUT(pServer,buf);
			send(sckClient, buf, iRet, 0);
			dwTotal += iRet;
		}
	}
//	TRACE(requestHeader.pszAuthEnd);

	// waiting for response from server
	iRet = pServer->RecvHeader(pServer,sckClient, buf, BUFFER_SIZE, &httpHeader);
	send(pThreadParam->sckClient, buf, iRet, 0);

	if(httpHeader.dwContSize==0)
	{
		goto _ThreadExit;
	}

	dwTotal = iRet-(httpHeader.pContBegin - buf);
	while(dwTotal<httpHeader.dwContSize && iRet)
	{
		iRet = recv(sckClient, buf, BUFFER_SIZE, 0);
		if(iRet==SOCKET_ERROR)
		{
			sprintf(pszMsg, "recv()=SOCKET_ERROR, WSAGetLastError()=%d\r\nListening Thread terminated.\r\n", WSAGetLastError());
			pServer->OUTPUT(pServer,pszMsg);
			goto _ThreadExit;
		}
		dwTotal += iRet;
		send(pThreadParam->sckClient, buf, iRet, 0);
	}

_ThreadExit:
	pServer->OUTPUT(pServer,"Working thread terminated.\r\n");
	closesocket(sckClient);
	closesocket(pThreadParam->sckClient);

_ThreadExit1:
	WaitForSingleObject(pServer->m_hConnectionCount, INFINITE);
	pServer->m_nConnectionCount--;
	ReleaseSemaphore(pServer->m_hConnectionCount, 1, NULL);

	delete pThreadParam;
	return 0;
}


int CProxyServer::RecvRequest(CProxyServer* pServer,SOCKET sck, char *buf, DWORD dwSize, REQUESTHEADER *pRequestHeader)
{
	char * pEnd, pszCode[32], pszText[32],
		*pAuthorization, *pRange, *pSize;
	int iRet;
	DWORD nTotal;

	pRequestHeader->dwFlag = REQUEST_UNAUTHORIZED;

	memset(buf, 0, dwSize);
	nTotal = 0;
	do{
		iRet = recv(sck, buf+nTotal, dwSize-nTotal-1, 0);
		if(iRet==SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}

		nTotal += iRet;
		buf[nTotal] = NULL;
		pEnd = strstr(buf, "\r\n\r\n");
		if(pEnd)
		{
			pAuthorization = strstr(buf, "Proxy-Authorization: ");
			if(!pAuthorization)
			{
				pRequestHeader->pszUserPass[0] = NULL;
				return 0;
			}
			sscanf(pAuthorization+21, "%*s%s", pszCode);
			Unbase64(pServer,pszText, pszCode);
			if(strcmp(pszText, "user:ag"))
			{
				strcpy(pRequestHeader->pszUserPass, pszText);
				return 0;
			}
			else
				pRequestHeader->dwFlag = 0;
			pRequestHeader->pszAuthBegin = pAuthorization;
			pRequestHeader->pszAuthEnd = strstr(pAuthorization, "\r\n")+2;
			pRequestHeader->pszVirtualRes = strstr(buf, "GET "); //"http://www.personalproxy.com/");
			if(pRequestHeader->pszVirtualRes)
			{
				if(!strncmp(pRequestHeader->pszVirtualRes+4, "http://www.personalproxy.com", 28))
				{
					pRequestHeader->dwFlag |= VIRTUAL_RESOURCE;
					sscanf(pRequestHeader->pszVirtualRes+4, "%s", pRequestHeader->pszUrl);
				}
			}

			pRange = strstr(buf, "RANGE: bytes");
			if(pRange)
			{
				pRange = strstr(pRange, "=");
				if(pRange)
				{
					sscanf(pRange+1, "%d", &pRequestHeader->dwRange);
					pRequestHeader->dwFlag |= PARTIAL_CONTENT;
				}
			}

			pSize = strstr(buf, "Content-Length: ");
			if(pSize)
			{
				sscanf(pSize+16, "%d", &pRequestHeader->dwContSize);
			}
			else
				pRequestHeader->dwContSize = 0;

			pRequestHeader->pszContBegin = pEnd+4;
			
			return nTotal;
		}
	}while(!pEnd && iRet);

	return 0;

}

void CProxyServer::HttpService(CProxyServer* pServer,char *pszUrl, SOCKET sck, char *buf, REQUESTHEADER *pRequestHeader)
{
    CString strServer, strObject;
	DWORD dwService, dwSize, dwFileLen;
	unsigned short nPort;
	int iRet;
	char pszFile[512], pszLine[512], pszType[16];
	char pszCommand[128], *pQuestionMark;
	FILE* pFileContent;

	AfxParseURL(pszUrl, dwService, strServer, strObject, nPort);
	if(strObject == "/")
		sprintf(pszFile, "%s%s", SITEBASE, "\\VirtualPage.html");
	else
		sprintf(pszFile, "%s%s", SITEBASE, strObject);

	UnCh(pServer,pszFile);
//		dwService		3
//		nPort			80
//	+	strObject		{"/count.dll?a=1&b=2"}
//	+	strServer		{"www.ms.com"}
//	+	this			0x0012fe74 {CTryDlg hWnd=0x01460920}

	if(strstr(pszFile, "myextension"))
	{
		pQuestionMark = strstr(pszFile, "?");
		if(pQuestionMark)
		{
			strcpy(pszCommand, pQuestionMark+1);
			*pQuestionMark = NULL;
		}
		else
			* pszCommand = NULL;
		HMODULE hModule = LoadLibrary(pszFile);
		if(!hModule)
		{
			pServer->OUTPUT(pServer,"requested file not found\r\n");
			strcpy(buf, "HTTP/1.1 404 Not Found\r\nTransfer-encoding: chunked\r\nContent-type: text/html\r\n\r\n<HTML><H1>File not found</H1></HTML>");
			send(sck, buf, strlen(buf), 0);
			return;
		}
		ProCommand = (int(*)(unsigned int, char*, char*))::GetProcAddress(hModule, "ProcessCommand");
		if(!ProCommand)
		{
			pServer->OUTPUT(pServer,"ProcessCommand==NULL\r\n");
			FreeLibrary(hModule);
			return;
		}
		ProCommand(sck, pszCommand, buf);
		FreeLibrary(hModule);
		return;
	}
	
	pFileContent = fopen(pszFile, "rb");
	if(!pFileContent)
	{
		pServer->OUTPUT(pServer,"requested file not found\r\n");
		strcpy(buf, "HTTP/1.1 404 Not Found\r\nTransfer-encoding: chunked\r\nContent-type: text/html\r\n\r\n<HTML><H1>File not found</H1></HTML>");
		send(sck, buf, strlen(buf), 0);
		return;
	}

	dwFileLen = _filelength(_fileno(pFileContent));
	if(pRequestHeader->dwFlag & PARTIAL_CONTENT)
	{
		strcpy(buf, "HTTP/1.1 206 Partial Content\r\n");
		strcat(buf, "Accept-ranges: bytes\r\n");

		dwSize = dwFileLen - pRequestHeader->dwRange;
		sprintf(pszLine, "Content-range: bytes %d-%d/%d\r\n", pRequestHeader->dwRange, dwFileLen-1, dwFileLen);
		strcat(buf, pszLine);

		fseek(pFileContent, pRequestHeader->dwRange, SEEK_SET);
	}
	else
	{
		strcpy(buf, "HTTP/1.1 200 OK\r\n");
		dwSize = dwFileLen;
	}
	
	sprintf(pszLine, "Content-length: %d\r\n", dwSize);
	strcat(buf, pszLine);
	strcpy(pszLine, "Proxy-agent: Ag-PersonalProxy/1.0\r\n");
	strcat(buf, pszLine);

	pServer->ContentType(pszFile, pszType);
	sprintf(pszLine, "Content-type: %s\r\n", pszType);
	strcat(buf, pszLine);
	
	strcat(buf, "\r\n");
	send(sck, buf, strlen(buf), 0);
	
	while(!feof(pFileContent))
	{
		dwSize = fread(buf, 1, BUFFER_SIZE, pFileContent);
		iRet = send(sck, buf, dwSize, 0);
		if(iRet == SOCKET_ERROR)
			break;
	}
	fclose(pFileContent);

	return;
}

void CProxyServer::Base64(CProxyServer* pServer,char *pszSource, char *pszCode)
{
    BYTE ot;
	int i;
	i=0;
	while(pszSource[i])
	{
		ot = (0xFC & pszSource[i])>>2; *pszCode = Table(ot); i++; pszCode++;
		if(!pszSource[i])
		{
			ot = (0x03 & pszSource[i-1])<<4; *pszCode = Table(ot); pszCode++;
			break;
		}
		ot = (0x03 & pszSource[i-1])<<4 | (0xF0 & pszSource[i])>>4; *pszCode = Table(ot); pszCode++; i++;
		if(!pszSource[i])
		{
			ot = (0x0F & pszSource[i-1])<<2; *pszCode = Table(ot); pszCode++;
			break;
		}
		ot = (0x0F & pszSource[i-1])<<2 | (0xC0 & pszSource[i])>>6; *pszCode = Table(ot); pszCode++;
		ot = 0x3F & pszSource[i]; *pszCode = Table(ot), i++; pszCode++;
	}
	*pszCode = NULL;
}

DWORD CProxyServer::RecvHeader(CProxyServer* pServer,SOCKET socket, char *buf, DWORD dwSize, HTTPHEADER *pHeader)
{
    char * pEnd, *pSize, *pType, *pLocation;
	int nRet, nTotal;
	
	memset(pHeader, 0, sizeof(HTTPHEADER));

	nTotal = 0;
	do{
		nRet = recv(socket, buf+nTotal, dwSize-nTotal, 0);
		if(nRet==SOCKET_ERROR)return SOCKET_ERROR;
		nTotal += nRet;
		buf[nRet] = NULL;
		pEnd = strstr(buf, "\r\n\r\n");
		if(pEnd)
		{
			pSize = strstr(buf, "Content-length: ");
			if(pSize)sscanf(pSize+16, "%ud", &(pHeader->dwContSize));
			else pHeader->dwContSize = -1;

			if(DWORD(pEnd-buf+4)<dwSize)
				pHeader->pContBegin = pEnd+4;
			else
				pHeader->pContBegin = NULL;
			sscanf(buf, "HTTP/%d.%d %d %s", &pHeader->nVerMajor, &pHeader->nVerMinor,
				&pHeader->dwHttpStatus, pHeader->pszStatus);

			pType = strstr(buf, "Content-type: ");
			if(pType)sscanf(pType+14, "%s", &(pHeader->pszType));

			pLocation = strstr(buf, "Location: ");
			if(pLocation)sscanf(pLocation+10, "s", &(pHeader->pszLocation));
			return nTotal;
		}
	}while(!pEnd && nRet);

	return nTotal;
}

void CProxyServer::Unbase64(CProxyServer* pServer,char *pszText, char *pszCode)
{
   int nLen, i;
	BYTE bySixBits0, bySixBits1, bySixBits2, bySixBits3, c;

	nLen = strlen(pszCode);
	for(i=0; i<nLen; )
	{
		bySixBits0 = GetSixBits(pszCode, i);
		if(i<nLen)bySixBits1 = GetSixBits(pszCode, i);
		if(i<nLen)bySixBits2 = GetSixBits(pszCode, i);
		if(i<nLen)bySixBits3 = GetSixBits(pszCode, i);

		c = bySixBits0<<2 | bySixBits1>>4; 
		*pszText = c; 
		pszText++;
		c = bySixBits1<<4 | bySixBits2>>2;
		*pszText = c; 
		pszText++;
		c = bySixBits2<<6 | bySixBits3;
		*pszText = c;
		pszText++;
	}
	*pszText = NULL;
}

void CProxyServer::UnCh(CProxyServer* pServer,char *pszStr)
{
    char pszLine[512];
	int i, j, nLen;
	BYTE b1, b2;

	i=0; j=0;
	nLen = strlen(pszStr);
	while(i<nLen)
	{
		if(pszStr[i]=='%')
		{
			i++;
			sscanf(pszStr+i, "%2X", &b1);
			i += 2;
		}
		else
		{
			pszLine[j++] = pszStr[i++];
			continue;
		}

		if(i>=nLen)
		{
			pszLine[j++] = b1;
			break;
		}

		if(pszStr[i]=='%')
		{
			i++;
			sscanf(pszStr+i, "%2X", &b2);
			i += 2;

			pszLine[j++] = b2;
			pszLine[j++] = b1;
		}
		else
		{
			pszLine[j++] = b1;
			pszLine[j++] = pszStr[i++];
		}
	}
	pszLine[j]=0;

	strcpy(pszStr, pszLine);
}

void CProxyServer::ContentType(char *pszFile, char *pszType)
{
   char drive[4], dir[MAX_PATH], fname[MAX_PATH], ext[16];
	_splitpath(pszFile, drive, dir, fname, ext);

	_strlwr(ext);
	if(strstr(pszFile, "multipart"))
		strcpy(pszType, "multipart/mixed");
	else if(!*ext)
		strcpy(pszType, "text/html");
	else if(!strcmp(ext, ".jpg"))
		strcpy(pszType, "image/jpeg");
	else if(!strcmp(ext, ".jpeg"))
		strcpy(pszType, "image/gif");
	else if(!strcmp(ext, ".htm"))
		strcpy(pszType, "text/html");
	else if(!strcmp(ext, ".html"))
		strcpy(pszType, "text/html");
	else if(!strcmp(ext, ".zip"))
		strcpy(pszType, "application/x-compressed");
	else if(!strcmp(ext, ".mpeg"))
		strcpy(pszType, "video/mpeg");
	else strcpy(pszType, "application/octet-stream");
}

BYTE CProxyServer::Table(BYTE ot)
{
    if(ot>=0 && ot<=25)return 'A' + ot;
	else if(ot>=26 && ot<=51)return 'a' + (ot-26);
	else if(ot>=52 && ot<=61)return '0' + (ot-52);
	else if(ot==62)return '+';
	else if(ot==63)return '/';
	else return '=';
}

BYTE CProxyServer::GetSixBits(char *pszStr, int &i)
{
    char c;

	while(1)
	{
		if(i>=strlen(pszStr))break;
		c = pszStr[i++];
		if(isupper(c)) return c-'A';
		else if(islower(c)) return c-'a'+26;
		else if(isdigit(c)) return c-'0'+52;
		else if(c=='+') return 62;
		else if(c=='/') return 63;
		else if(c=='=') return 0;
	}
	return 0;
}
