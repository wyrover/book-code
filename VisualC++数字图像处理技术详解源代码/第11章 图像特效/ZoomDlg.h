#pragma once


// CZoomDlg 对话框

#include "PreviewDlg.h"

class CZoomDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CZoomDlg)

public:
	int		m_nWidth;		// 输出图像的宽度
	int		m_nHeight;		// 输出图像的高度
	BOOL	m_bIsPro;		// 是否限定长宽比例
	int		m_nType;		// 缩放模式

	CZoomDlg(CWnd* pParent = NULL); 
	virtual ~CZoomDlg();

	virtual BOOL OnInitDialog();

	// 消息响应函数
	afx_msg void OnBnClickedPrev();
	afx_msg void OnEnChangeWidth();
	afx_msg void OnEnChangeHeight();
	afx_msg void OnBnClickedPro();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	int		m_nSrcWidth;			// 原始图像的宽度
	int		m_nSrcHeight;			// 原始图像的高度

	void Refresh();					// 重新进行图像缩放并刷新预览窗口
	void RefreshSize(bool width);	// 自动计算输出图像的大小

private:
	bool			m_bModify;		// 图像是否被修改

	// 对话框数据
	enum { IDD = IDD_ZOOM };
};
