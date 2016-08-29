// TreeNdx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeIndex dialog

class CTreeIndex : public CIndexPage
{
	DECLARE_DYNCREATE(CTreeIndex)

// Construction
public:
	CTreeIndex();
	~CTreeIndex();

// Dialog Data
	//{{AFX_DATA(CTreeIndex)
	CListBox	c_Index;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTreeIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ixlist * getIndexTable();
	// Generated message map functions
	//{{AFX_MSG(CTreeIndex)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
