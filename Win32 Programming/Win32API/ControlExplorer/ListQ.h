// ListQ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListQuery dialog

class CListQuery : public CListOps
{
	DECLARE_DYNCREATE(CListQuery)

// Construction
public:
	CListQuery();
	~CListQuery();

// Dialog Data
	//{{AFX_DATA(CListQuery)
	enum { IDD = IDD_LISTBOXQUERY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListQuery)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CListQuery)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetanchorindex();
	afx_msg void OnGetcount();
	afx_msg void OnGetcursel();
	afx_msg void OnGethorizontalextent();
	afx_msg void OnGetlocale();
	afx_msg void OnGetselcount();
	afx_msg void OnGetselitems();
	afx_msg void OnGettopindex();
	afx_msg void OnGetcaretindex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
