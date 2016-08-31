// Editndx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditIndex dialog

class CEditIndex : public CIndexPage
{
	DECLARE_DYNCREATE(CEditIndex)

// Construction
public:
	CEditIndex();
	~CEditIndex();

// Dialog Data
	//{{AFX_DATA(CEditIndex)
	CListBox	c_Index;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ixlist * getIndexTable();
	// Generated message map functions
	//{{AFX_MSG(CEditIndex)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
