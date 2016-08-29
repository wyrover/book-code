// IndexPg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndexPage dialog

typedef struct {
		int id;  // message ID
		int ctlid; // control ID (for setting focus)
		int page;  // page id
	       } ixlist;

class CIndexPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CIndexPage)

// Construction
public:
	CIndexPage();
	~CIndexPage();

// Dialog Data
	//{{AFX_DATA(CIndexPage)
	enum { IDD = IDD_INDEXPAGE };
	CListBox	c_Index;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIndexPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ixlist * getIndexTable();
	void gotoPage(int page, int ctlid);
	// Generated message map functions
	//{{AFX_MSG(CIndexPage)
	afx_msg void OnDblclkIndex();
	virtual BOOL OnInitDialog();
	afx_msg void OnShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
