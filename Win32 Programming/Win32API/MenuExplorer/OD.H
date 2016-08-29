// OD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COD dialog

class COD : public CPropertyPage
{
	DECLARE_DYNCREATE(COD)

// Construction
public:
	COD();
	~COD();

// Dialog Data
	//{{AFX_DATA(COD)
	enum { IDD = IDD_OWNERDRAW };
	CEdit	c_MenuText;
	CTinyButton	c_Add1;
	CMenuCheck	c_CheckMarks;
	CSysColor	c_HiliteMenuColor;
	CTinyButton	c_Add;
	CSysColor	c_TextColor;
	CTinyButton	c_Remove;
	CSysColor	c_MenuColor;
	CSysColor	c_HiliteTextColor;
	CSysColor	c_GrayColor;
	CIntEdit	c_ActualCheckWidth;
	CStatic	c_SubWidth;
	CSpinButtonCtrl	c_SpinItemWidth;
	CSpinButtonCtrl	c_SpinItemHeight;
	CIntEdit	c_ItemWidth;
	CIntEdit	c_ItemHeight;
	CButton	c_DoSubWidth;
	CStatic	c_CheckMarkDimensions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COD)
	afx_msg void OnDosubwidth();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd1();
	afx_msg void OnChangeMenutext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
