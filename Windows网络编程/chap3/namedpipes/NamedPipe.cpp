// NamedPipe.cpp: implementation of the CNamedPipe class.
//
// Author:    Emil Gustafsson (e@ntier.se), 
//            NTier Solutions (www.ntier.se)
// Created:   2000-01-25
// Copyright: This code may be reused and/or editied in any project
//            as long as this original note (Author and Copyright)
//            remains in the source files.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NamedPipe.h"

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

	HANDLE m_hOutPipe = NULL;
	HANDLE m_hInPipe  = NULL;
	HANDLE m_hListner = NULL;
}

CNamedPipe::~CNamedPipe()
{
	if (m_hOutPipe != NULL && m_hOutPipe != INVALID_HANDLE_VALUE)
		CloseHandle(m_hOutPipe);
	if (m_hInPipe != NULL && m_hInPipe != INVALID_HANDLE_VALUE)
		CloseHandle(m_hInPipe);

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
	if (fCallBack == NULL)
		return false;
	if (bAsServer)
	{ // This instance is a server, hence create the pipes.
		m_hInPipe = CreateNamedPipe(
			GetRealPipeName(true).c_str(),
			PIPE_ACCESS_INBOUND,
			PIPE_WAIT,
			1,
			PIPE_BUF_SIZE,
			PIPE_BUF_SIZE,
			PIPE_TIMEOUT,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;

		m_hOutPipe = CreateNamedPipe(
			GetRealPipeName(false).c_str(),
			PIPE_ACCESS_OUTBOUND,
			PIPE_WAIT,
			1,
			PIPE_BUF_SIZE,
			PIPE_BUF_SIZE,
			PIPE_TIMEOUT,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
	}
	else
	{ // obviously a client, just open existing pipes.
		m_hInPipe = CreateFile(
			GetRealPipeName(false).c_str(),
			GENERIC_READ,
			0, //i.e. No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hInPipe == NULL || m_hInPipe == INVALID_HANDLE_VALUE)
			return false;

		m_hOutPipe = CreateFile(
			GetRealPipeName(true).c_str(),
			GENERIC_WRITE,
			0, //i.e. No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hOutPipe == NULL || m_hOutPipe == INVALID_HANDLE_VALUE)
			return false;
	}
	ListnerParam* pLP = new ListnerParam;
	pLP->funcCallBack = fCallBack;
	pLP->hPipe = m_hInPipe;
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

	return true;
}

/*static*/
DWORD WINAPI CNamedPipe::ListnerProc(LPVOID lpParameter)
{
	ListnerParam* pLP = (ListnerParam*)lpParameter;
	if (pLP == NULL)
		return 1;

	DWORD dwRetVal = 0;
	for (bool bContinue = true; bContinue && dwRetVal == 0; Sleep(0))
	{
		char buf[PIPE_BUF_SIZE];
		DWORD dwRead;
		BOOL bOK = ReadFile(pLP->hPipe,buf,PIPE_BUF_SIZE,&dwRead,NULL);
		if (dwRead == 0)
			continue;
		if (!bOK)
			dwRetVal = 2;
		string szMsg = buf;
		if (szMsg == pLP->szStopCmd)
			bContinue = false;
		pLP->funcCallBack(szMsg);
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

string CNamedPipe::GetRealPipeName(bool bIsServerInPipe)
{
	string szRetVal = m_szFullPipeName;
	szRetVal += bIsServerInPipe?"_IN":"_OUT";
	return szRetVal;
}

bool CNamedPipe::Send(string szMsg)
{
	DWORD dwSent;
	BOOL bOK = WriteFile(m_hOutPipe,szMsg.c_str(),szMsg.length()+1,&dwSent,NULL);
	if (!bOK || (szMsg.length()+1) != dwSent)
		return false;
	return true;
}
	
