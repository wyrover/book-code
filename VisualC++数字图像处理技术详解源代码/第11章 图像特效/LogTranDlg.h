#pragma once

#include "PreviewDlg.h"
#include "afxwin.h"

// CLogTranDlg 对话框

class CLogTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CLogTranDlg)

public:
	int		m_nPosA;		// 滑块A关联变量
	int		m_nPosB;		// 滑块B关联变量
	int		m_nA;			// 参数a
	int		m_nB;			// 参数b
	CStatic m_stiPrev;		// 变换函数预览框

	// 标准构造析构函数
	CLogTranDlg(CWnd* pParent = NULL); 
	virtual ~CLogTranDlg();

	void Refresh();						// 刷新预览框
	virtual BOOL OnInitDialog();		// 对话框初始化时显示预览对话框

	afx_msg void OnEnChangeEdita();		// 改变参数A时的响应函数
	afx_msg void OnEnChangeEditb();		// 改变参数B时的响应函数
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
										// 滑块滑动的响应函数
	afx_msg void OnPaint();				// 对话框重绘函数
	afx_msg void OnBnClickedSave();		// “保存”按钮响应函数
	afx_msg void OnBnClickedOk();		// “确定”按钮的响应函数

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CPreviewDlg*	dlg;		// 预览对话框
	bool			m_bModify;	// 图像是否被修改

	enum { IDD = IDD_LOGTRAN };	// 对话框数据
};
