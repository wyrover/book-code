// Header.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderTest dialog

class CHeaderTest : public CPropertyPage
{
	DECLARE_DYNCREATE(CHeaderTest)

// Construction
public:
	CHeaderTest();
	~CHeaderTest();

// Dialog Data
	//{{AFX_DATA(CHeaderTest)
	enum { IDD = IDD_HEADER };
	CNumericEdit	c_Width;
	CNumericEdit	c_Item;
	CButton	c_NoAnimation;
	CMessageLog	c_Messages;
	CStatic	c_HeadingSpace;
	CButton	c_HDS_Horz;
	CButton	c_HDS_Hidden;
	CButton	c_HDS_Buttons;
	CHeaderCtrl c_Header;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHeaderTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void logTrack(NMHDR * hdr);

	void createHeaderControl();
	void setStyle(BOOL checked, DWORD newstyle);
	void logSetWindowLong(DWORD style);
	void logNotify(NMHDR * hdr);
	void addStyle(CString & flags, BOOL hasit, int styleid);

	// Generated message map functions
	//{{AFX_MSG(CHeaderTest)
	afx_msg void OnHdsButtons();
	afx_msg void OnHdsHidden();
	afx_msg void OnHdsHorz();
	afx_msg void OnSendmessages();
	virtual BOOL OnInitDialog();
	afx_msg void OnHeaderBeginTrack(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderDividerDblClick(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderEndTrack(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderItemChanged(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderItemChanging(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderClick(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderDblClick(NMHDR * hdr, LRESULT * result);
	afx_msg void OnHeaderTrack(NMHDR * hdr, LRESULT * result);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
