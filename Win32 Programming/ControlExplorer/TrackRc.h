// TrackRc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackRect dialog

class CTrackRect : public CTrackOps
{
	DECLARE_DYNCREATE(CTrackRect)

// Construction
public:
	CTrackRect();
	~CTrackRect();

// Dialog Data
	//{{AFX_DATA(CTrackRect)
	enum { IDD = IDD_TRACKBAR_RECT };
	CNumericEdit	c_ThumbTop;
	CNumericEdit	c_ThumbRight;
	CNumericEdit	c_ThumbLeft;
	CNumericEdit	c_ThumbBottom;
	CNumericEdit	c_ChannelRight;
	CNumericEdit	c_ChannelLeft;
	CNumericEdit	c_ChannelBottom;
	CNumericEdit	c_ChannelTop;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrackRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTrackRect)
	afx_msg void OnGetthumbrect();
	afx_msg void OnGetchannelrect();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
