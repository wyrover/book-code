// ListNdx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListIndex dialog

class CListIndex : public CIndexPage
{
	DECLARE_DYNCREATE(CListIndex)

// Construction
public:
	CListIndex();
	~CListIndex();

// Dialog Data
	//{{AFX_DATA(CListIndex)
	CListBox	c_Index;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ixlist * getIndexTable();
	// Generated message map functions
	//{{AFX_MSG(CListIndex)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
