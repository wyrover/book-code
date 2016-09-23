#pragma once
#include "afxcmn.h"


class CDriverModule : public CDialog
{
	DECLARE_DYNAMIC(CDriverModule)

public:
	CDriverModule(CWnd* pParent = NULL);   
	virtual ~CDriverModule();
	enum { IDD = IDD_DLG_DRIVER_MODULE };

public:
	VOID InitControl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
};
