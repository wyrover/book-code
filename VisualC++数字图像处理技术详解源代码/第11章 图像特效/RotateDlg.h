#pragma once

#include "PreviewDlg.h"

// CRotateDlg 对话框

class CRotateDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRotateDlg)

public:
	int m_nAngle;		// 旋转角度
	int m_nPos;			// 旋转角度滑块关联
	int m_nType;		// 旋转模式

	CRotateDlg(CWnd* pParent = NULL); 
	virtual ~CRotateDlg();

	void Refresh();
	virtual BOOL OnInitDialog();

	// 消息响应函数
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnEnChangeAngle();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	bool	m_bModify;	// 图像是否被修改

	// 对话框数据
	enum { IDD = IDD_ROTATE };
};