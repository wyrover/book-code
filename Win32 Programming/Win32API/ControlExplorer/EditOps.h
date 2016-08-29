// EditOps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditOps dialog

class CEditOps : public CLoggingPage
{
// Construction
public:
	DECLARE_DYNCREATE(CEditOps)
	CEditOps(int id);
	CEditOps();    // standard constructor

// Dialog Data
	CEdit * edit;

	//{{AFX_DATA(CEditOps)
	enum { IDD = IDD_EDITTOOLS };
	CSpinButtonCtrl	c_SpinLowSel;
	CIntEdit	c_LowSel;
	CSpinButtonCtrl	c_SpinHighSel;
	CIntEdit	c_HighSel;
	CButton	        c_ScrollSel;
	
	CTinyButton	c_GetThumb;
	CSpinButtonCtrl	c_SpinLineFromCharVal;
	CSpinButtonCtrl c_SpinLimit;
	CIntEdit	c_LineFromCharVal;
	CSpinButtonCtrl	c_SpinGetlineVal;
	CIntEdit	c_GetlineVal;
	CIntEdit	c_LimitVal;
	CTinyButton	c_GetSelText;
	CTinyButton	c_GetLineCount;
	CButton c_Undoable;
	CButton	c_EOLFlag;
	CHexDisplay	c_Edit;
	CTinyButton	c_SetPasswordChar;
	CEdit	c_PasswordChar;
	CButton	c_ReadOnlyFlag;
	CButton	c_ModifyFlag;
	CButton	c_Hex;
	CTinyButton	c_FmtLines;
	CTinyButton	c_GetMargins;
	CTinyButton	c_EM_Undo;
	CTinyButton	c_WM_Undo;
	CTinyButton	c_WM_Paste;
	CTinyButton	c_WM_Cut;
	CTinyButton	c_WM_COPY;
	CTinyButton	c_SetMargins;
	CTinyButton	c_EmptyUndoBuffer;
	CTinyButton	c_CanUndo;
	CTinyButton	c_SetText;

	CSpinButtonCtrl	c_SpinPosX;
	CSpinButtonCtrl	c_SpinPosY;
	CIntEdit	c_PosX;
	CIntEdit	c_PosY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditOps)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void showResult(int id);
	void showResult_d(int id, int result);
	void showResult_DW(int id, DWORD result);
	void showResult_c(int id, TCHAR result);
	void showResult_wd(int id, int w);
	void showResult_wd_d(int id, int w, int result);
	void showResult_wc(int id, TCHAR w);
	void showResult_dd_d(int id, int w, int l, int result);

	void addMessage(CString s);
	virtual void enableControls();
	CString charToStr(TCHAR ch);
	DWORD getFirstVisibleLine();
	int getLineFromChar(int pos);
	DWORD getSel();
	

	// Generated message map functions
	//{{AFX_MSG(CEditOps)
	afx_msg void OnHex();
	afx_msg void OnGetlinecount();
	afx_msg void OnCanundo();
	afx_msg void OnEmptyundobuffer();
	afx_msg void OnFmtlines();
	afx_msg void OnGetfirstvisibleline();
	afx_msg void OnGetlimittext();
	afx_msg void OnSetlimittext();
	afx_msg void OnGetline();
	afx_msg void OnGetmargins();
	afx_msg void OnGetmodify();
	afx_msg void OnGetpasswordchar();
	afx_msg void OnGetrect();
	afx_msg void OnGetsel();
	afx_msg void OnGetseltext();
	afx_msg void OnGetthumb();
	afx_msg void OnWmUndo();
	afx_msg void OnEmUndo();
	afx_msg void OnWmSettext();
	afx_msg void OnWmPaste();
	afx_msg void OnWmGettextlength();
	afx_msg void OnWmGettext();
	afx_msg void OnWmCut();
	afx_msg void OnWmCopy();
	afx_msg void OnWmClear();
	afx_msg void OnSetsel();
	afx_msg void OnSetrectnp();
	afx_msg void OnSetrect();
	afx_msg void OnSetreadonly();
	afx_msg void OnSetpasswordchar();
	afx_msg void OnSetmodify();
	afx_msg void OnSetmargins();
	afx_msg void OnScrollcaret();
	afx_msg void OnScroll();
	afx_msg void OnReplacesel();
	afx_msg void OnPosfromchar();
	afx_msg void OnLinescroll();
	afx_msg void OnLineindex();
	afx_msg void OnLinefromchar();
	afx_msg void OnLinelength();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePasswordChar();
	afx_msg void OnChangeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
