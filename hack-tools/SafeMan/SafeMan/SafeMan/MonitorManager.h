#pragma once
#include "afxwin.h"

// CMonitorManager 对话框
class CMonitorManager : public CDialog
{
	DECLARE_DYNAMIC(CMonitorManager)

public:
	CMonitorManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMonitorManager();

// 对话框数据
	enum { IDD = IDD_MONITORMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_Count;
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedProcessmonitorcontrol();
	// 获取ZwCreateProcessEx的真实地址
	DWORD GetZwCreateProcessAddress(void);

	CButton m_ButtonProcessMonitor;
	int m_pProcessMonitor;		//ProcessMonitorOn or Off
	CStatic m_Status;
	CBrush m_brush;
};
