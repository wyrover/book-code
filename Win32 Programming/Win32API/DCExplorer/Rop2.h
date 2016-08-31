// Rop2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRop2 dialog

class CRop2 : public CDialog
{
// Construction
public:
	CRop2(CWnd* pParent = NULL);   // standard constructor
	CString ropname;
	int rop;

// Dialog Data
	//{{AFX_DATA(CRop2)
	enum { IDD = IDD_ROP2 };
	CStatic	c_RGB;
	CRopCodes	c_Rop2;
	CSample	c_Top;
	CSample	c_Left;
	CSample	c_Sample;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRop2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRop2)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokRop2();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnColorChange(WPARAM wParam, LPARAM);
	afx_msg LRESULT OnColorClear(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};
