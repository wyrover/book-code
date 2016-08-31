// Text.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CText dialog

class CText : public CPropertyPage
{
	DECLARE_DYNCREATE(CText)

// Construction
public:
	CText();
	~CText();
	CDCValues * dcv;
	

// Dialog Data
	//{{AFX_DATA(CText)
	enum { IDD = IDD_TEXT };
	CIntEdit	c_TextCharExtra;
	CSpinButtonCtrl	c_SpinCharExtra;
	CStatic	c_TextSample;
	CColor	c_TextColor;
	CButton	c_Transparent;
	CButton	c_Opaque;
	CButton	c_StockFont;
	CIDCombo	c_StockFonts;
	CButton	c_ChangeFont;
	CStatic	c_FontName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CText)
	public:
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont * font;
	CPen  * pen;
	CBrush * brush;

	void updateControls();
	CString getWeightString(int weight);
	void setFontName();
	//{{AFX_MSG(CText)
	afx_msg void OnChangefont();
	afx_msg void OnCustomfont();
	afx_msg void OnStockfont();
	afx_msg void OnSelendokStockfonts();
	afx_msg void OnTextcolor();
	afx_msg void OnOpaque();
	afx_msg void OnTransparent();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelendokTextcolor();
	afx_msg void OnChangeTextcharextra();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
