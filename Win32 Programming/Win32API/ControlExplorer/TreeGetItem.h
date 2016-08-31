// TreeGetItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeGetItem dialog

class CTreeGetItem : public CTV_ITEM
{
	DECLARE_DYNCREATE(CTreeGetItem)

// Construction
public:
	CTreeGetItem();
	~CTreeGetItem();

// Dialog Data
	//{{AFX_DATA(CTreeGetItem)
	enum { IDD = IDD_TREEGETITEM };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTreeGetItem)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTreeGetItem)
	afx_msg void OnGetitem();
	afx_msg void OnSetitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
