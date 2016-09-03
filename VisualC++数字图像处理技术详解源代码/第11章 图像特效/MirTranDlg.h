#pragma once

// CMirTranDlg 对话框

#include "PreviewDlg.h"

class CMirTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CMirTranDlg)

public:
	int m_nType;			// 镜像变换类型

	CMirTranDlg(CWnd* pParent = NULL);
	virtual ~CMirTranDlg();

	void Refresh();					// 重新进行镜像变换并刷新预览窗口
	virtual BOOL OnInitDialog();	// 初始化对话框

	// 消息响应函数
	afx_msg void OnBnClickedMirrorH();
	afx_msg void OnBnClickedMirrorV();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV 支持

private:
	CPreviewDlg*	dlg;		// 预览对话框
	bool			m_bModify;	// 图像是否被修改

	// 对话框数据
	enum { IDD = IDD_MIRROR };
};
