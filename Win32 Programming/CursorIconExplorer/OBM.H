// OBM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COBM dialog

class COBM : public CPropertyPage
{
        DECLARE_DYNCREATE(COBM)
// Construction
public:
	COBM();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COBM)
	enum { IDD = IDD_OBM };
	CStatic	c_Bitmap;
	CButton	c_AutoCopy;
	CButton	c_Copy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COBM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HBITMAP bm;
	// Generated message map functions
	//{{AFX_MSG(COBM)
	afx_msg void OnCopy();
	afx_msg void OnAutocopy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OBMSelect(UINT id);
	DECLARE_MESSAGE_MAP()

};
