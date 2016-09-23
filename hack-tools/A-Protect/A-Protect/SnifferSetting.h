#pragma once


// CSnifferSetting 对话框

class CSnifferSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSnifferSetting)

public:
	CSnifferSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSnifferSetting();

// 对话框数据
	enum { IDD = IDD_DLG_SNIFFERSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strData;
	CString m_strDataDescription;
};
