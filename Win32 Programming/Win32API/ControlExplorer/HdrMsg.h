// HdrMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderMsg dialog

class CHeaderMsg : public CLoggingPage
{
	DECLARE_DYNCREATE(CHeaderMsg)

// Construction
public:
	CHeaderMsg();
	~CHeaderMsg();
	CHeaderCtrl * c_Header;

// Dialog Data
	//{{AFX_DATA(CHeaderMsg)
	enum { IDD = IDD_HEADERMSGS };
	CTinyButton	c_Plus12;
	CTinyButton	c_ComputeWidth;
	CNumericEdit	c_ItemCount;
	CStatic	c_c_lParam;
	CNumericEdit	c_lParam;
	CEdit	c_Text;
	CStatic	c_c_Text;
	CSpinButtonCtrl	c_SpinItem;
	CNumericEdit	c_Item;
	CButton	c_HDM_SETITEM;
	CButton	c_HDM_INSERTITEM;
	CButton	c_HDM_GETITEM;
	CButton	c_HDM_DELETEITEM;
	CButton	c_HDI_WIDTH;
	CButton	c_HDI_TEXT;
	CButton	c_HDI_LPARAM;
	CButton	c_HDI_HEIGHT;
	CButton	c_HDI_FORMAT;
	CButton	c_HDI_BITMAP;
	CButton	c_HDF_STRING;
	CButton	c_HDF_RTLREADING;
	CButton	c_HDF_RIGHT;
	CButton	c_HDF_OWNERDRAW;
	CButton	c_HDF_LEFT;
	CButton	c_HDF_CENTER;
	CButton	c_HDF_BITMAP;
	CStatic	c_c_hbm;
	CBitmapSelect	c_hbm;
	CButton	c_c_fmt;
	CStatic	c_c_cxy;
	CNumericEdit	c_cxy;
	CNumericEdit	c_cchTextMax;
	CStatic	c_c_cchTextMax;
	//}}AFX_DATA
#ifdef HBT_SPRING
	CTinyButton c_Springy;
#endif


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHeaderMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableControls();
	void hdlToControls();
	void controlsToHdl();
	virtual void addMessage(CString s);
	UINT getMask();
	int getFmt();
	CString textarea;
	void computeWidth(int bias);
	void forceNewBitmap();
	void preDeleteOldBitmap();
	void postDeleteOldBitmap();
	int desired_bitmap;
	void selectMatchingBitmap();

	HD_ITEM hdl;
	HBITMAP deletion_candidate;

	// Generated message map functions
	//{{AFX_MSG(CHeaderMsg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHdiBitmap();
	afx_msg void OnHdiFormat();
	afx_msg void OnHdiHeight();
	afx_msg void OnHdiLparam();
	afx_msg void OnHdiText();
	afx_msg void OnHdiWidth();
	afx_msg void OnHdmGetitemcount();
	afx_msg void OnHdmDeleteitem();
	afx_msg void OnHdmGetitem();
	afx_msg void OnHdmInsertitem();
	afx_msg void OnHdmSetitem();
	afx_msg void OnComputewidth();
	afx_msg void OnSetfont();
	afx_msg void OnPlus12();
	//}}AFX_MSG
#ifdef HBT_SPRING
	afx_msg void OnSpringy();
#endif
	DECLARE_MESSAGE_MAP()

};
