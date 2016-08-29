// EditStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditState dialog

class CEditState : public CEditOps
{
	DECLARE_DYNCREATE(CEditState)

// Construction
public:
	CEditState();
	~CEditState();

	CTinyButton c_GetLimitText;
	CTinyButton c_SetLimitText;
// Dialog Data
	//{{AFX_DATA(CEditState)
	enum { IDD = IDD_EDITSTATE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditState)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditState)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
