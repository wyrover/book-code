// MenuInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuInfo dialog

class CMenuInfo : public CPropertyPage
{
 	DECLARE_DYNCREATE(CMenuInfo)
// Construction
public:
	CMenuInfo();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMenuInfo)
	enum { IDD = IDD_MENUINFO };
	CBitmaps	c_Bitmaps;
	CComboBox	c_MenuText;
	CIntEdit	c_Position;
	CEdit	c_Result;
	CIntEdit	c_MenuHandle;
	CStatic	c_c_dwTypeData;
	CStatic	c_c_cch;
	CStatic	c_c_wID;
	CStatic	c_c_dwItemData;
	CSpinButtonCtrl	c_SpinPosition;
	CSpinButtonCtrl	c_SpinCommand;
	CButton	c_ByCommand;
	CButton	c_ByPosition;
	CStatic	c_c_hSubmenu;
	CIntEdit	c_Command;
	CStatic	c_c_hbmpUnchecked;
	CStatic	c_c_hbmpChecked;
	CButton	c_c_fState;
	CButton	c_c_fType;
	CButton	c_MFT_NoBreak;
	CMenuList	c_MenuList;
	CMenuCheck	c_hbmpUnchecked;
	CMenuCheck	c_hbmpChecked;
	CTinyButton	c_SetMenuInfo;
	CTinyButton	c_GetMenuInfo;
	CIntEdit	c_fState;
	CIntEdit	c_wID;
	CButton	c_MIIM_Type;
	CButton	c_MIIM_Submenu;
	CButton	c_MIIM_State;
	CButton	c_MIIM_ID;
	CButton	c_MIIM_Data;
	CButton	c_MIIM_CheckMarks;
	CButton	c_MFT_String;
	CButton	c_MFT_Separator;
	CButton	c_MFT_RightJustify;
	CButton	c_MFT_RadioCheck;
	CButton	c_MFT_OwnerDraw;
	CButton	c_MFT_MenuBreak;
	CButton	c_MFT_MenuBarBreak;
	CButton	c_MFT_Bitmap;
	CButton	c_MFS_Hilite;
	CButton	c_MFS_Grayed;
	CButton	c_MFS_Disabled;
	CButton	c_MFS_Default;
	CButton	c_MFS_Checked;
	CIntEdit	c_hSubmenu;
	CIntEdit	c_fType;
	CIntEdit	c_fMask;
	CIntEdit	c_dwTypeData;
	CIntEdit	c_dwItemData;
	CIntEdit	c_cch;
	CIntEdit	c_cbSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuInfo)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	MENUITEMINFO info;
	void controlsToInfo(BOOL fetch);
	void infoToControls();

	void MFTtoControl();
	void MFStoControl();
	void MIIMtoControl();
	DWORD getMFTfromControls();
	DWORD getMFSfromControls();
	DWORD getMIIMfromControls();

	void enableControls();
	HTREEITEM tree_root;
	HTREEITEM tree_main;

	BOOL changed;


	void loadMenuDisplay();
	int getItemPos(HTREEITEM ti);
	void logError(DWORD lasterror);
	void logEvent(int msgid, HMENU hmenu, int id, BOOL result);

	TCHAR menutext[256];

	// Generated message map functions
	//{{AFX_MSG(CMenuInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCch();
	afx_msg void OnChangeDwitemdata();
	afx_msg void OnGetmenuinfo();
	afx_msg void OnSelendokHbmpchecked();
	afx_msg void OnSelendokHbmpunchecked();
	afx_msg void OnSelchangedMenulist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMfsChecked();
	afx_msg void OnMfsDefault();
	afx_msg void OnMfsDisabled();
	afx_msg void OnMfsGrayed();
	afx_msg void OnMfsHilite();
	afx_msg void OnMftBitmap();
	afx_msg void OnMftMenubarbreak();
	afx_msg void OnMftMenubreak();
	afx_msg void OnMftOwnerdraw();
	afx_msg void OnMftRadiocheck();
	afx_msg void OnMftRightjustify();
	afx_msg void OnMftSeparator();
	afx_msg void OnMftString();
	afx_msg void OnMiimCheckmarks();
	afx_msg void OnMiimData();
	afx_msg void OnMiimId();
	afx_msg void OnMiimState();
	afx_msg void OnMiimSubmenu();
	afx_msg void OnMiimType();
	afx_msg void OnSetmenuinfo();
	afx_msg void OnChangeWid();
	afx_msg void OnMftNobreak();
	afx_msg LRESULT OnChange(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
