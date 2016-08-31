// TreeMisc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeMisc dialog

class CTreeMisc : public CTreeOps
{
	DECLARE_DYNCREATE(CTreeMisc)

// Construction
public:
	CTreeMisc();
	~CTreeMisc();

// Dialog Data
	//{{AFX_DATA(CTreeMisc)
	enum { IDD = IDD_TREEMISC };
	CButton	c_TextOnly;
	CStatic	c_c_Top;
	CNumericEdit	c_Top;
	CStatic	c_c_Right;
	CNumericEdit	c_Right;
	CButton	c_c_Rect;
	CStatic	c_c_Left;
	CNumericEdit	c_Left;
	CTinyButton	c_GetItemRect;
	CStatic	c_c_Bottom;
	CNumericEdit	c_Bottom;
	CNumericEdit	c_Count;
	CButton	c_Cancel;
	CButton	c_c_Flag;
	CButton	c_TVGN_CARET;
	CButton	c_TVGN_DROPHILITE;
	CButton	c_TVGN_FIRSTVISIBLE;
	CComboBox	c_hItem;
	CNumericEdit	c_Indent;
	CSpinButtonCtrl	c_SpinIndent;
	CTinyButton	c_DeleteItem;
	CTinyButton	c_EditLabel;
	CTinyButton	c_EndEditLabelNow;
	CTinyButton	c_GetCount;
	CTinyButton	c_GetIndent;
	CTinyButton	c_SelectItem;
	CTinyButton	c_SetIndent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTreeMisc)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableControls();
	void loadComboFromTree(CComboBox * box);

	// Generated message map functions
	//{{AFX_MSG(CTreeMisc)
	afx_msg void OnDeleteitem();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditlabel();
	afx_msg void OnEndeditlabelnow();
	afx_msg void OnSelendokHitem();
	afx_msg void OnGetcount();
	afx_msg void OnGetindent();
	afx_msg void OnSelectitem();
	afx_msg void OnSetindent();
	afx_msg void OnGetitemrect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
