// NamedPipe1.cpp: implementation of the CNamedPipe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "namedPipe.h"
#include "NamedPipe1.h"

// NamedPipe.cpp: implementation of the CNamedPipe class.

//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNamedPipe::CNamedPipe()
{
	string m_szPipeName = "";
	string m_szPipeHost = ".";
	string m_szFullPipeName = "\\\\.\\PIPE\\";
	HANDLE m_hPipe = NULL;

	HANDLE m_hListner = NULL;
}

CNamedPipe::~CNamedPipe()
{
	if (m_hPipe != NULL && m_hPipe != INVALID_HANDLE_VALUE)
		CloseHandle(m_hPipe);

	DWORD dwTemp;
	if (GetExitCodeThread(m_hListner,&dwTemp))
	{
		if (dwTemp == STILL_ACTIVE)
			TerminateThread(m_hListner,3);
	}
	if (m_hListner != NULL && m_hListner != INVALID_HANDLE_VALUE)
		CloseHandle(m_hListner);
}

bool CNamedPipe::Initialize(bool bAsServer, 
							string szStopListnenCmd,
							void (WINAPI *fCallBack)(string buf))
{
	if (bAsServer && (fCallBack == NULL))
		return false;
	if (bAsServer)
	{ // This instance is a server, hence create the pipes.
	
		m_hPipe = CreateNamedPipe(
			GetPipeName().c_str(),
			PIPE_ACCESS_INBOUND,
			PIPE_NOWAIT,
			1,
			PIPE_BUF_SIZE,
			PIPE_BUF_SIZE,
			PIPE_TIMEOUT,
			NULL);
		if (m_hPipe == NULL || m_hPipe == INVALID_HANDLE_VALUE)
			return false;

		ListnerParam* pLP = new ListnerParam;
		pLP->funcCallBack = fCallBack;
		pLP->hPipe = m_hPipe;
		pLP->szStopCmd = szStopListnenCmd;
		m_hListner = CreateThread(
			NULL,
			0,
			&ListnerProc,
			(LPVOID)pLP,
			0,
			&m_dwListnerThreadId);
		if (m_hListner == NULL || m_hListner == INVALID_HANDLE_VALUE)
			return false;
	}
	else
	{ // obviously a client, just open existing pipes.
		m_hPipe = CreateFile(
			GetPipeName().c_str(),
			GENERIC_WRITE,
			0, //No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	
		ASSERT(m_hPipe !=INVALID_HANDLE_VALUE);
		if (m_hPipe == NULL || m_hPipe == INVALID_HANDLE_VALUE)
			
			return false;
	}

	return true;
}

/*static*/
DWORD WINAPI CNamedPipe::ListnerProc(LPVOID lpParameter)
{
	ListnerParam* pLP = (ListnerParam*)lpParameter;
	if (pLP == NULL)
		return 1;

	DWORD dwRetVal = 0;
	bool bContinue = true;
	while (bContinue && dwRetVal == 0) 
	{
		char buf[PIPE_BUF_SIZE];
		DWORD dwRead;
		BOOL bOK = ReadFile(pLP->hPipe,buf,PIPE_BUF_SIZE,&dwRead,NULL);
		if (dwRead == 0)
		{
			Sleep(100);
			continue;
		}
		
		if (!bOK) //¶ÁÎÄ¼þÊ§°Ü
			dwRetVal = 2;
		string szMsg = buf;
		if (szMsg == pLP->szStopCmd)//¶Áµ½ÖÕÖ¹·û
			bContinue = false;
		pLP->funcCallBack(szMsg);
		Sleep(100);
	}
	delete pLP;
	return dwRetVal;
}

void CNamedPipe::SetPipeName(string szName, string szHost/* = "."*/)
{
	m_szPipeName = szName;
	m_szPipeHost = szHost;
	m_szFullPipeName = "\\\\";
	m_szFullPipeName += m_szPipeHost;
	m_szFullPipeName += "\\PIPE\\";
	m_szFullPipeName += m_szPipeName;
}



bool CNamedPipe::Send(string szMsg)
{
	DWORD dwSent;
	BOOL bOK = WriteFile(m_hPipe,szMsg.c_str(),szMsg.length()+1,&dwSent,NULL);
	if (!bOK || (szMsg.length()+1) != dwSent)
		return false;
	return true;
}
	
