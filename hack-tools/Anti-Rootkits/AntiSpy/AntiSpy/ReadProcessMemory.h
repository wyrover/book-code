#pragma once
#include "afxwin.h"

class CReadProcessMemory : public CDialog
{
	DECLARE_DYNAMIC(CReadProcessMemory)
public:
	BOOL SendIoControlCode(ULONG_PTR ulControlCode);
	BOOL ReadMemory();
public:
	ULONG_PTR m_ProcessID;

public:
	CReadProcessMemory(CWnd* pParent = NULL);   
	virtual ~CReadProcessMemory();

	enum { IDD = IDD_DLG_READ_PROCESS_MEMORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRead();
	CString m_Address;
	ULONG   m_Length;
	CString m_ProcessMemoryData;
	CString m_Binary;
	CString m_Address_Edit;
	CEdit m_Address_Control;
};
