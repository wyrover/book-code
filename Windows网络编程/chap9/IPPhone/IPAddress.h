#if !defined(AFX_IPADDRESS_H__EDF5DE41_0E46_11D4_8D3A_00002100A114__INCLUDED_)
#define AFX_IPADDRESS_H__EDF5DE41_0E46_11D4_8D3A_00002100A114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPAddress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPAddress dialog

class CIPAddress : public CDialog
{
// Construction
public:
	char* ip;
	CIPAddress(CWnd* pParent = NULL);   // standard constructor
	~CIPAddress();
// Dialog Data
	//{{AFX_DATA(CIPAddress)
	enum { IDD = IDD_IPADDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPAddress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIPAddress)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPADDRESS_H__EDF5DE41_0E46_11D4_8D3A_00002100A114__INCLUDED_)
