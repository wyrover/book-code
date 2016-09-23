#pragma once
#include "afxcmn.h"
#include "ProcessManager.h"
#include "DriverModule.h"
#include <windows.h>  
#include <winsvc.h>  
#include <conio.h>  

enum TableList
{
	ProcessManager = 0,
	DriverModule,
	HookMangager,
};



class CAntiSpyDlg : public CDialogEx
{
public:
	CAntiSpyDlg(CWnd* pParent = NULL);
	~CAntiSpyDlg();
	enum { IDD = IDD_ANTISPY_DIALOG };



public:
	VOID InitControl();
	HANDLE OpenDevice(LPCTSTR wzLinkPath);
	BOOL LoadNTDriver(WCHAR* lpszDriverName,WCHAR* lpszDriverPath);
	BOOL UnloadNTDriver(WCHAR* szSvrName);

protected:
	CProcessManager m_ProcessManagerDlg;
	CDriverModule   m_DriverModuleDlg;
	ULONG           m_CurSelTab;
	CDialog*        m_Dlg[5];


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL FreeSource(LPCTSTR pwzResName,LPCTSTR pwzResType,LPCTSTR wzFileName) ;
};
