// ListBox.h : header file
//
typedef struct {
	CListBox * lb;   // control reference
	int    CtlID;    // control ID
	CString CtlName; // control name string
	int     width;   // control name width
	       } ctldesc;


/////////////////////////////////////////////////////////////////////////////
// CListBoxTest dialog

class CListBoxTest : public CPropertyPage
{
	DECLARE_DYNCREATE(CListBoxTest)

// Construction
public:
	CListBoxTest();
	~CListBoxTest();

// Dialog Data
	//{{AFX_DATA(CListBoxTest)
	enum { IDD = IDD_LISTBOX };
	CButton	c_NoAnimation;
	CButton c_WSBorder;
	CButton c_Enable;
	CButton c_LBSNotify;
	CButton c_LBSMultiColumn;
	CButton c_LBSNoIntegralHeight;
	CButton c_LBSSort;
	CButton c_LBSUseTabStops;
	CButton c_LBSWantKeyboardInput;
	CButton c_LBSDisableNoScroll;
	CStatic	c_c_Single;
	CStatic	c_c_None;
	CStatic	c_c_Multiple;
	CStatic	c_c_Extended;
	CMessageLog	c_Messages;
	CListBox	c_Single;
	CListBox	c_None;
	CListBox	c_Multiple;
	CListBox	c_Extended;
	//}}AFX_DATA

	ctldesc ctls[4];  
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListBoxTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont * orgfont;
	ctldesc * findCtl(int id);
	void recreateWindow(CListBox * box, int id, DWORD style);
	void recreateWindows();

	void logNotification(int id, int notification);
	void logToItem(int msg, UINT key, CListBox * lb, UINT nIndex, int result);
	void logSelection(ctldesc * desc);
	CString findNotify(int notification);
	void addSampleStrings(int ctl, int sortctl);
	void setParms(int index, CListBox * lb, CStatic * lbl);
	int getMaxCtlWidth();
	void showResult_vkey_LBERR(int msg, CListBox * lb, CString CtlName, TCHAR w, int pos, int result);

	// Generated message map functions
	//{{AFX_MSG(CListBoxTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnable();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkExtended();
	afx_msg void OnKillfocusExtended();
	afx_msg void OnSelcancelExtended();
	afx_msg void OnSelchangeExtended();
	afx_msg void OnSetfocusExtended();
	afx_msg void OnDblclkMultiple();
	afx_msg void OnKillfocusMultiple();
	afx_msg void OnSelcancelMultiple();
	afx_msg void OnSelchangeMultiple();
	afx_msg void OnSetfocusMultiple();
	afx_msg void OnDblclkNone();
	afx_msg void OnKillfocusNone();
	afx_msg void OnSelcancelNone();
	afx_msg void OnSelchangeNone();
	afx_msg void OnSetfocusNone();
	afx_msg void OnDblclkSingle();
	afx_msg void OnKillfocusSingle();
	afx_msg void OnSelcancelSingle();
	afx_msg void OnSelchangeSingle();
	afx_msg void OnSetfocusSingle();
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	afx_msg void OnSendmessages();
	afx_msg void OnLbsDisablenoscroll();
	afx_msg void OnLbsMulticolumn();
	afx_msg void OnLbsNointegralheight();
	afx_msg void OnLbsNotify();
	afx_msg void OnLbsSort();
	afx_msg void OnLbsUsetabstops();
	afx_msg void OnLbsWantkeyboardinput();
	afx_msg void OnWsBorder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
