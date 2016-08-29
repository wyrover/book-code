// ListRect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListboxRect dialog

class CListboxRect : public CListOps
{
	DECLARE_DYNCREATE(CListboxRect)

// Construction
public:
	CListboxRect();
	~CListboxRect();

// Dialog Data
	//{{AFX_DATA(CListboxRect)
	enum { IDD = IDD_LISTBOXRECT };
	CTinyButton	c_SetitemHeight;
	CIntEdit	c_uly;
	CIntEdit	c_ulx;
	CSpinButtonCtrl	c_Spinuly;
	CSpinButtonCtrl	c_Spinulx;
	CSpinButtonCtrl	c_Spinlry;
	CSpinButtonCtrl	c_Spinlrx;
	CSpinButtonCtrl	c_SpinIndex;
	CIntEdit	c_lry;
	CIntEdit	c_lrx;
	CIntEdit	c_ItemHeight;
	CIntEdit	c_Index;
	CSpinButtonCtrl	c_SpinItemHeight;
	CTinyButton	c_GetItemRect;
	CTinyButton	c_GetItemHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListboxRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CListboxRect)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetitemheight();
	afx_msg void OnGetitemrect();
	afx_msg void OnSetitemheight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
