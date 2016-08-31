// EditMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditMapping dialog

class CEditMapping : public CEditOps
{
	DECLARE_DYNCREATE(CEditMapping)

// Construction
public:
	CEditMapping();
	~CEditMapping();

// Dialog Data
	//{{AFX_DATA(CEditMapping)
	enum { IDD = IDD_EDITMAPPING };
	CStatic	c_e_Pos;
	CButton	c_c_Pos;
	CTinyButton	c_CharFromPos;
	CTinyButton	c_PosFromChar;
	CIntEdit	c_LineVal;
	CButton	c_SetSel;
	CSpinButtonCtrl	c_SpinColumnVal;
	CSpinButtonCtrl	c_SpinLineVal;
	CIntEdit	c_Column;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditMapping)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditMapping)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetfirstvisibleline();
	afx_msg void OnColumn();
	afx_msg void OnLinefromchar();
	afx_msg void OnLineindex();
	afx_msg void OnPosfromchar();
	afx_msg void OnCharfrompos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
