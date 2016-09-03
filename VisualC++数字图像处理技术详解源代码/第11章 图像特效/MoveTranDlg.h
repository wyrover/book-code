#pragma once

// CMoveTranDlg 对话框

#include "PreviewDlg.h"

class CMoveTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CMoveTranDlg)

public:
	int		m_nType;		// 平移变换类型
	int		m_nX;			// 水平偏移量
	int		m_nY;			// 垂直偏移量

	CMoveTranDlg(CWnd* pParent = NULL); 
	virtual ~CMoveTranDlg();

	void Refresh();						// 重新进行平移变换并刷新预览窗口
	virtual BOOL OnInitDialog();		// 初始化对话框

	// 消息响应函数
	afx_msg void OnBnClickedRadioNormal();
	afx_msg void OnBnClickedRadioSize();
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CPreviewDlg*	dlg;		// 预览对话框
	bool			m_bModify;	// 图像是否被修改

	// 对话框数据
	enum { IDD = IDD_MOVETRAN };
};
