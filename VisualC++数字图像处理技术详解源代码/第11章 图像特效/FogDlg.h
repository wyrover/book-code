#pragma once


// CFogDlg 对话框

class CFogDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CFogDlg)

public:
	int m_nPos;		// 滑块关联变量
	int m_nValue;	// 雾化参数
	int m_nType;	// 雾化类型

	CFogDlg(CWnd* pParent = NULL); 
	virtual ~CFogDlg();

	void Refresh();
	virtual BOOL OnInitDialog();

	// 消息响应函数
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	bool	m_bModify;	// 图像是否修改

	// 对话框数据
	enum { IDD = IDD_FOG };
};