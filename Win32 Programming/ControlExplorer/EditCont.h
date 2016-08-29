// EditCont.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditContents dialog

class CEditContents : public CEditOps
{
	DECLARE_DYNCREATE(CEditContents)

// Construction
public:
	CEditContents();
	~CEditContents();

// Dialog Data
	//{{AFX_DATA(CEditContents)
	enum { IDD = IDD_EDITCONTENTS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditContents)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditContents)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
