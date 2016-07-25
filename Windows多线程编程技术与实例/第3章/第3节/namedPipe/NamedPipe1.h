// NamedPipe1.h: interface for the CNamedPipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDPIPE1_H__D497FC47_C9B5_4780_9254_D769A1766E72__INCLUDED_)
#define AFX_NAMEDPIPE1_H__D497FC47_C9B5_4780_9254_D769A1766E72__INCLUDED_

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

	HANDLE m_hPipe;
// 	HANDLE m_hInPipe;
	HANDLE m_hListner;
	DWORD  m_dwListnerThreadId;

public:
	void   SetPipeName(string szName, string szHost = ".");
	string GetPipeName() { return m_szFullPipeName; }
//	string GetRealPipeName(bool bIsServerInPipe);

	bool Send(string szMsg);
};


#endif // !defined(AFX_NAMEDPIPE1_H__D497FC47_C9B5_4780_9254_D769A1766E72__INCLUDED_)
