// PenSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPenSel dialog

class CPenSel : public CPropertyPage
{
 	DECLARE_DYNCREATE(CPenSel)

// Construction
public:
	CPenSel();
	~CPenSel();
	CDCValues * dcv;

// Dialog Data
	//{{AFX_DATA(CPenSel)
	enum { IDD = IDD_PEN };
	CSpinButtonCtrl	c_MiterSpin;
	CIntEdit	c_MiterAngle;
	CFloatEdit	c_MiterLimit;
	CSpinButtonCtrl	c_MiterAngleSpin;
	CButton	c_c_MiterLimit;
	CStatic	c_c_MiterAngle;
	CButton	c_c_Width;
	CButton	c_Transparent;
	CButton	c_Opaque;
	CStatic	c_c_PenStyle;
	CStatic	c_c_LineJoin;
	CStatic	c_c_BrushStyle;
	CStatic	c_c_EndCaps;
	CStatic	c_c_Color;
	CIntEdit	c_PenOrgY;
	CIntEdit	c_PenOrgX;
	CSpinButtonCtrl	c_PenSpin;
	CIntEdit	c_PenWidth;
	CButton	c_Geometric;
	CButton	c_Cosmetic;
	CPenSample	c_Sample;
	CIDCombo	c_StockPens;
	CButton	c_StockPen;
	CButton c_SolidPen;
	CColor	c_SolidPens;
	CPenStyle c_PenStyles;
	CEndCaps  c_EndCaps;
	CLineJoin c_LineJoins;
	CBrushStyle c_BrushStyle;
	//}}AFX_DATA


// Overrides
	virtual BOOL OnSetActive();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenSel)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void updateSample();
	void updateControls();
	CPen * current_pen;
	void setPen();
	void changePenStyle();
	BOOL initialized;   // for avoid buddy control assertions

	// Generated message map functions
	//{{AFX_MSG(CPenSel)
	afx_msg void OnPencolor();
	afx_msg void OnSelendcancelSolidpens();
	virtual BOOL OnInitDialog();
	afx_msg void OnStockpen();
	afx_msg void OnSelendokEndcaps();
	afx_msg void OnSelendokJoins();
	afx_msg void OnSelendokStockpens();
	afx_msg void OnGeometric();
	afx_msg void OnCosmetic();
	afx_msg void OnSelendokBrushstyle();
	afx_msg void OnSelendokLinejoin();
	afx_msg void OnSelendokPenstyles();
	afx_msg void OnChangePenwidth();
	afx_msg void OnSelendokSolidpens();
	afx_msg void OnOpaque();
	afx_msg void OnTransparent();
	afx_msg void OnChangeMiterangle();
	afx_msg void OnChangeMiterlimit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
