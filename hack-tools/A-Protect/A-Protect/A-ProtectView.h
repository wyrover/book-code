
// AProtectView.h : CMyAProtectView 类的接口
//

#pragma once
#include "A-ProtectDoc.h"
#include "resource.h"
#include "Afxcmn.h"
#include "MyList.h"
#include "stackthread.h"

class CMyAProtectView : public CFormView
{
public: // 仅从序列化创建
	CMyAProtectView();
	DECLARE_DYNCREATE(CMyAProtectView)

public:
	enum{ IDD = IDD_MYAProtect_FORM };

// 特性
public:
	CMyAProtectDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CMyAProtectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	HICON m_hIcon;
	CTreeCtrl m_TreeCtrl;
	CMyList m_ListCtrl;
	afx_msg void OnOpenKernelType();
	//afx_msg void OnWindowsOverhead();
	//afx_msg void OnCancelTheOverhead();
	afx_msg void OnShutdownKernelType();
	afx_msg void OnProtectProcess();
	afx_msg void OnUnProtectProcess();
	afx_msg void OnBooting();
	afx_msg void OnCancelBooting();
	afx_msg void OnOpenUrl();
	afx_msg void OnClose();
	afx_msg void OnAbout();
	afx_msg void OnOpenSrcDownLoad();
	afx_msg void OnGetPDB();
	afx_msg void OnKernelBSOD();

	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void SSDTHook();
	afx_msg void SSDTAllHook();
	afx_msg void UnHookSSDTHookSelect();
	afx_msg void UnHookSSDTHookAll();
	afx_msg void OnCopySSDTDataToClipboard();

	afx_msg void ShadowSSDTHook();
	afx_msg void ShadowSSDTAllHook();
	afx_msg void UnHookShadowSSDTHookSelect();
	afx_msg void UnHookShadowSSDTHookAll();
	afx_msg void OnCopyShadowSSDTDataToClipboard();

	afx_msg void OnKernelHook();
	afx_msg void OnUnHookKernelHookSelect();
	afx_msg void OnByPassKernelHook();
	afx_msg void OnCopyKernelHookDataToClipboard();
	afx_msg void OnAllKernelHook();
	afx_msg void OnStopHookScan();

	afx_msg void OnKernelModule();
	afx_msg void OnKernelModuleTrust();
	afx_msg void OnDumpMemoryDataToFile();
	afx_msg void OnCopyKernelModuleDataToClipboard();

	afx_msg void OnKernelThreadList();
	afx_msg void OnClearKernelThreadLog();

	afx_msg void OnSystemThreadList();
	afx_msg void OnKillSystemThread();
	afx_msg void OnSuspendThread();
	afx_msg void OnResumeThread();

	afx_msg void OnFilterDriverList();
	afx_msg void OnDeleteSelectFilterDriver();

	afx_msg void OnFsdHookList();
	afx_msg void OnFsdHookResetOne();
	afx_msg void OnCopyFsdDataToClipboard();

	afx_msg void OnTcpipHookList();
	afx_msg void OnTcpipHookResetOne();
	afx_msg void OnCopyTcpipDataToClipboard();

	afx_msg void OnNsipHookList();
	afx_msg void OnNsipHookResetOne();
	afx_msg void OnCopyNsipDataToClipboard();

	afx_msg void OnKbdclassHookList();
	afx_msg void OnKbdclassHookResetOne();
	afx_msg void OnCopyKbdclassDataToClipboard();

	afx_msg void OnMouclassHookList();
	afx_msg void OnMouclassHookResetOne();
	afx_msg void OnCopyMouclassDataToClipboard();

	afx_msg void OnAtapiHookList();
	afx_msg void OnAtapiHookResetOne();
	afx_msg void OnCopyAtapiDataToClipboard();

	afx_msg void OnDpcTimerList();
	afx_msg void OnKillDpcTimer();
	afx_msg void OnCopyDpcTimerDataToClipboard();

	afx_msg void OnSystemNotifyList();
	afx_msg void OnKillSystemNotify();
	afx_msg void OnCopySystemNotifyDataToClipboard();

	afx_msg void OnQueryProcess();
	afx_msg void OnQueryProcessTrust();
	afx_msg void OnProcessmodule();
	afx_msg void OnProcessmoduleTrust();
	afx_msg void OnDLLModuleWithOutTrust();

	afx_msg void OnProcessHandle();
	afx_msg void OnProcessThread();
	afx_msg void OnKillProcess();
	afx_msg void OnKillProcessDeleteFile();
	afx_msg void OnProcessVerify();
	afx_msg void OnTrustProcFile();	
	afx_msg void OnCopyProcessDataToClipboard();
	afx_msg void OnCopyProcessMD5ToClipboard();
	afx_msg void OnSearchDllModule();
	afx_msg void OnDLLModuleNotInSystem();
	afx_msg void OnDLLModuleInSystem();
	afx_msg void OnSearchProcessWithGoogle();
	afx_msg void OnGetProcessFilePath();
	afx_msg void OnSuspendProcess();
	afx_msg void OnResumeProcess();
	
	afx_msg void OnGetServicesList();
	afx_msg void OnGetServicesListByTrust();
	afx_msg void OnGetDepthServicesList();
	afx_msg void OnServicesStartOnly();
	afx_msg void OnServicesUnTrustOnly();
	afx_msg void OnStartServices();
	afx_msg void OnStopServices();
	afx_msg void OnDeleteServices();
	afx_msg void OnManualServices();
	afx_msg void OnAutoServices();
	afx_msg void OnDisableServices();

	afx_msg void OnMsgTcpView();
	afx_msg void OnMsgTcpProKill();

	afx_msg void OnListLog();
	afx_msg void GetSaveFileLog(WCHAR *lpwzFilePath);
	afx_msg void OnClearListLog();
	afx_msg void OnConnectScan();
	afx_msg void OnSaveToFile();

	afx_msg void OnIoTimerList();
	afx_msg void OnBnStopIoTimer();
	afx_msg void OnBnStartIoTimer();
	afx_msg void OnCopyIoTimerDataToClipboard();

	afx_msg void OnMessageHookFunction();


	/*afx_msg void OnBnClickedQzdeletefile();
	afx_msg void OnBnClickedDisdriver();
	afx_msg void OnBnClickedDisprocess();
	afx_msg void OnBnClickedDisfile();
	afx_msg void OnBnClickedShutdownSystem();
	afx_msg void OnBnClickedServicesReset();
	afx_msg void OnBnClickedDisKernelThread();
	afx_msg void OnUninstall360();*/

	afx_msg void OnTCPSnifferSetting();

	afx_msg void OnBnWorkQueue();
	afx_msg void OnBnKillWorkQueue();
	afx_msg void OnSuspendWorkQueueThread();
	afx_msg void OnResumeWorkQueueThread();

	afx_msg void OnBnPhysical();

	int m_case;
	CImageList ImgList;
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);

	int m_iItem;//用来记录在第几行上单击的，也可改为第几列单击的

	CString m_strData;
	CString m_strDataDescription;
	afx_msg void OnStackthread();
	afx_msg void OnBtnKernelData();
	afx_msg void OnOpenurl();
	afx_msg void OnSelectmoduleinlinehook();
	afx_msg void OnSelectAnyModule();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnSelectExport();
};

#ifndef _DEBUG  // AProtectView.cpp 中的调试版本
inline CMyAProtectDoc* CMyAProtectView::GetDocument() const
   { return reinterpret_cast<CMyAProtectDoc*>(m_pDocument); }
#endif

