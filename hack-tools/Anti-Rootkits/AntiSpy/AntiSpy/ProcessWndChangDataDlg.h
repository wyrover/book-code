#pragma once


// CProcessWndChangDataDlg ¶Ô»°¿ò

class CProcessWndChangDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessWndChangDataDlg)

public:
	CProcessWndChangDataDlg(CWnd* pParent = NULL);  
	virtual ~CProcessWndChangDataDlg();


	enum { IDD = IDD_DLG_PROCESS_WND_CHANGE_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit;
};
