// EditRect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditRect dialog

class CEditRect : public CEditOps
{
	DECLARE_DYNCREATE(CEditRect)

// Construction
public:
	CEditRect();
	~CEditRect();

// Dialog Data
	//{{AFX_DATA(CEditRect)
	enum { IDD = IDD_EDITRECTANGLES };
	CButton	c_ECUseFontInfo;
	CButton	c_ECRightMargin;
	CButton	c_ECLeftMargin;
	CIntEdit	c_uly;
	CIntEdit	c_ulx;
	CSpinButtonCtrl	c_Spinuly;
	CSpinButtonCtrl	c_Spinulx;
	CSpinButtonCtrl	c_SpinRight;
	CSpinButtonCtrl	c_Spinlry;
	CSpinButtonCtrl	c_Spinlrx;
	CSpinButtonCtrl	c_SpinLeft;
	CTinyButton	c_SetRectNP;
	CTinyButton	c_SetRect;
	CTinyButton	c_SetMargins;
	CIntEdit	c_Right;
	CButton	c_MarginCaption;
	CIntEdit	c_lry;
	CIntEdit	c_lrx;
	CIntEdit	c_Left;
	CTinyButton	c_GetRect;
	CTinyButton	c_GetMargins;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void doRect(int msg, int id);

	// Generated message map functions
	//{{AFX_MSG(CEditRect)
	afx_msg void OnGetrect();
	afx_msg void OnGetmargins();
	afx_msg void OnSetmargins();
	afx_msg void OnSetrect();
	afx_msg void OnSetrectnp();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
