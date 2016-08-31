// Trackndx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackIndex dialog

class CTrackIndex : public CIndexPage
{
	DECLARE_DYNCREATE(CTrackIndex)

// Construction
public:
	CTrackIndex();
	~CTrackIndex();

// Dialog Data
	//{{AFX_DATA(CTrackIndex)
	CListBox	c_Index;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrackIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ixlist * getIndexTable();
	// Generated message map functions
	//{{AFX_MSG(CTrackIndex)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
