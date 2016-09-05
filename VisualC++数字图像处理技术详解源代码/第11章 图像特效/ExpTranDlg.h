#pragma once
#include "afxwin.h"

#include "PreviewDlg.h"

// CExpTranDlg 对话框

class CExpTranDlg : public CDialog
{
	DECLARE_DYNAMIC(CExpTranDlg)

public:
	CExpTranDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExpTranDlg();

// 对话框数据
	enum { IDD = IDD_EXPTRAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void Refresh();
	int m_nPosA;
	int m_nA;
	int m_nPosB;
	int m_nB;
	int m_nPosC;
	int m_nC;
	CStatic m_stiPrev;

	CPreviewDlg* dlg;
	bool	m_bModify;
	afx_msg void OnEnChangeEdita();
	afx_msg void OnEnChangeEditb();
	afx_msg void OnEnChangeEditc();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();
};
