// PingThread.h: interface for the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
#define AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ping.h"
class CPingThread  
{
public:
	CPingThread();
	virtual ~CPingThread();

	UINT	m_dwID;
	HANDLE	m_hThread;
	HANDLE	m_hKillEvent;
	HANDLE	m_hSignalEvent;

	static UINT __stdcall  ThreadProc(void* pThis);


	void StartPing(UINT nRetries,CString strHost,HWND hWnd);

	CPing	m_ping;
	CString m_strHost;
	HWND	m_hWnd;
	UINT	m_nRetries;

};

#endif // !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
