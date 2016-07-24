// NamedPipe.h: interface for the CNamedPipe class.
//
// Author:    Emil Gustafsson (e@ntier.se), 
//            NTier Solutions (www.ntier.se)
// Created:   2000-01-25
// Copyright: This code may be reused and/or editied in any project
//            as long as this original note (Author and Copyright)
//            remains in the source files.
//////////////////////////////////////////////////////////////////////

#if !defined(NAMEDPIPE_H)
#define NAMEDPIPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

#define PIPE_BUF_SIZE 1024
#define PIPE_TIMEOUT  (120*1000) /*120 seconds*/

class CNamedPipe  
{
public:
	CNamedPipe();
	virtual ~CNamedPipe();

	bool Initialize(bool bAsServer, string szStopListnenCmd, void (WINAPI *fCallBack)(string buf));

	static DWORD WINAPI ListnerProc(LPVOID lpParameter);
	struct ListnerParam
	{
		HANDLE hPipe;                            // Handle to pipe to listne to...
		string szStopCmd;                        // Stop listen when this is read.
		void (WINAPI *funcCallBack)(string buf); // Call this function for every successful read operation.
	};
protected:
	string m_szPipeName;
	string m_szPipeHost;
	string m_szFullPipeName;

	HANDLE m_hOutPipe;
	HANDLE m_hInPipe;
	HANDLE m_hListner;
	DWORD  m_dwListnerThreadId;

public:
	void   SetPipeName(string szName, string szHost = ".");
	string GetPipeName() { return m_szFullPipeName; }
	string GetRealPipeName(bool bIsServerInPipe);

	bool Send(string szMsg);
};

#endif // !defined(NAMEDPIPE_H)
