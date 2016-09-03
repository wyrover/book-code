#pragma once
#include "afxwin.h"


// CPowerTranDlg 对话框

#include "PreviewDlg.h"

class CPowerTranDlg : public CDialog
{
	DECLARE_DYNAMIC(CPowerTranDlg)

public:
	CPowerTranDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowerTranDlg();

// 对话框数据
	enum { IDD = IDD_POWERTRAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nPosC;
	int m_nPosR;
	int m_nC;
	int m_nR;
	CStatic m_stiPrev;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CPreviewDlg* dlg;
	bool	m_bModify;

	void Refresh();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditb();
	afx_msg void OnEnChangeEditc();
	afx_msg void OnEnChangeEditr();
	int m_nPosB;
	int m_nB;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();
};
