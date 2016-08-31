// DropDown.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDropDownTest dialog

class CDropDownTest : public CPropertyPage
{
	DECLARE_DYNCREATE(CDropDownTest)

// Construction
public:
	CDropDownTest();
	~CDropDownTest();

// Dialog Data
	//{{AFX_DATA(CDropDownTest)
	enum { IDD = IDD_DROPDOWN };
	CButton	c_SmartDropdown;
	CButton	c_Enable;
	CButton	c_ExtendedUI;
	CButton	c_Wide;
	CButton	c_Simple;
	CStatic	c_DropSize;
	CButton	c_DropList;
	CButton	c_DropDown;
	CButton	c_Delete;
	CComboBox * c_Combo;
	CComboBox	c_ComboSimple;
	CComboBox	c_ComboDropDown;
	CComboBox	c_ComboDropDownList;
	CButton	c_Add;
	CMessageLog	c_Messages;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDropDownTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString getComboType(CComboBox * cb);
	int getMaxCtlWidth();
	void logNotification(CComboBox * cb, int notification);
	void logToItem(int msg, UINT key, CComboBox * cb, UINT nIndex, int result);
	void logSelection(CComboBox * cb);
	void enableControls();
	void enableAddDelete();
	void changeTo(CComboBox * newCombo);
	CString findNotify(int notification);
	void copyStrings(CComboBox * cb);
	BOOL hasEdit();
	BOOL hasDrop();

	// Generated message map functions
	//{{AFX_MSG(CDropDownTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnCloseupCombo();
	afx_msg void OnDblclkCombo();
	afx_msg void OnDropdownCombo();
	afx_msg void OnEditchangeCombo();
	afx_msg void OnEditupdateCombo();
	afx_msg void OnErrspaceCombo();
	afx_msg void OnKillfocusCombo();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnSelendcancelCombo();
	afx_msg void OnSelendokCombo();
	afx_msg void OnSetfocusCombo();
	afx_msg void OnDelete();
	afx_msg void OnDropdown();
	afx_msg void OnDroplist();
	afx_msg void OnSimple();
	afx_msg void OnWide();
	afx_msg void OnExtendedui();
	afx_msg void OnEnable();
	afx_msg void OnSendmessages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
