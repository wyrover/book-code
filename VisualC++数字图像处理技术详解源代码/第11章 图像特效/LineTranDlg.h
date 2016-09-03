#pragma once


// CLineTranDlg 对话框

#include "PreviewDlg.h"
#include "afxwin.h"

class CLineTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CLineTranDlg)

public:
	CStatic m_stiPrev;			// 变换函数预览框
	double	m_dSlope;			// 斜率
	double	m_dIntercept;		// 截距

	// 标准构造析构函数
	CLineTranDlg(CWnd* pParent = NULL); 
	virtual ~CLineTranDlg();

	void Refresh();					// 刷新预览框
	virtual BOOL OnInitDialog();	// 对话框初始化时显示预览对话框

	afx_msg void OnEnChangeSlope();	// 改变斜率时的响应函数
	afx_msg void OnEnChangeIntercept();
									// 改变截距时的响应函数
	afx_msg void OnBnClickedSave();	// “保存”按钮响应函数
	afx_msg void OnBnClickedOk();	// “确定”按钮的响应函数
	afx_msg void OnBnClickedCancel();
									// “取消”按钮的响应函数
	afx_msg void OnPaint();			// 对话框重绘函数

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CPreviewDlg*	dlg;		// 预览对话框
	bool			m_bModify;	// 图像是否被修改

	// 对话框数据
	enum { IDD = IDD_LINETRAN };
};
