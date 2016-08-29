// ComboNdx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboIndex dialog

class CComboIndex : public CIndexPage
{
	DECLARE_DYNCREATE(CComboIndex)

// Construction
public:
	CComboIndex();
	~CComboIndex();

// Dialog Data
	//{{AFX_DATA(CComboIndex)
	CListBox	c_Index;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CComboIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ixlist * getIndexTable();
	// Generated message map functions
	//{{AFX_MSG(CComboIndex)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
