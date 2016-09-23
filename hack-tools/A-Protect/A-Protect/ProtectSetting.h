#pragma once


// CProtectSetting 对话框

class CProtectSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CProtectSetting)

public:
	CProtectSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProtectSetting();

// 对话框数据
	enum { IDD = IDD_PROTECTSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDeletefile();
	afx_msg void OnBnClickedDisloaddriver();
	afx_msg void OnBnClickedDiscreateprocess();
	afx_msg void OnBnClickedDiscreatefile();
	afx_msg void OnBnClickedDissrvreset();
	afx_msg void OnBnClickedDiskernelthread();
	afx_msg void OnBnClickedDisSetWindowsHook();
	afx_msg void OnBnClickedDisDllFuck();
	afx_msg void OnBnClickedShutdownsystem();
	afx_msg void OnBnClickedUninstall360();
};
