// Callback.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallback dialog

class CCallback : public CTV_ITEM
{
	DECLARE_DYNCREATE(CCallback)

// Construction
public:
	CCallback();   // standard constructor
	~CCallback();
	virtual void enableControls();
	static int UWM_RECORD_DISPINFO;
	CWnd * controller;

// Dialog Data
	TV_ITEM * item; // reference to item that we will be modifying
	//{{AFX_DATA(CCallback)
	enum { IDD = IDD_TREECALLBACK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallback)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCallback)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
