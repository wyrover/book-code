#pragma once
#include "afxwin.h"
#include "resource.h"


// CPrevDlg 对话框

class CPrevDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CPrevDlg)

public:
	// 控件变量
	CString		m_strNum;				// 图像数量信息文本
	CScrollBar	m_ScrollBar;			// 水平滚动条
	CStatic		m_staPrev;				// 图像预览框

	CPrevDlg(CWnd* pParent = NULL);		// 标准构造函数
	virtual ~CPrevDlg();

	void Redraw(void);					// 预览框重绘函数

	// 消息响应函数
	afx_msg void OnPaint();				// 绘制对话框
	afx_msg void OnBnClickedOpen();		// “打开”按钮响应函数
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
										// 水平滚动条响应函数

	enum { IDD = IDD_PREV };			// 对话框数据

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	int				m_nPicNum;			// 当前目录的文件个数
	CString			m_strPath;			// 当前打开目录
	CStringArray	m_FilesNameAry;		// 保存当前目录下所有文件名的容器
	int				m_nPos;				// 当前文件在容器中的索引
};
