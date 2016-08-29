// Bkg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBkg dialog

class CBkg : public CPropertyPage
{
	DECLARE_DYNCREATE(CBkg)

// Construction
public:
	CBkg();
	~CBkg();
	CDCValues * dcv;

// Dialog Data
	//{{AFX_DATA(CBkg)
	enum { IDD = IDD_BACKGROUND };
	CSysColor	c_WindowColor;
	CSpinButtonCtrl	c_SpinY;
	CSpinButtonCtrl	c_SpinX;
	CHatchBrushSample	c_HatchBrushSample;
	CStatic	c_c_Origin;
	CHatchSample	c_HatchSample;
	CColor	c_BkColor;
	CButton	c_HatchBrush;
	CButton	c_SolidBrush;
	CButton	c_StockBrush;
	CPoly	c_Poly;
	CIntEdit	c_BrushOrgY;
	CIntEdit	c_BrushOrgX;
	CRopCodes	c_RasterOp;
	CButton	c_Winding;
	CButton	c_Transparent;
	CStock	c_StockBrushes;
	CSysColor c_SolidBrushes;
	CButton	c_Opaque;
	CBrushStyle	c_HatchBrushes;
	CButton	c_Alternate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBkg)
	public:
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void updateControls();
	void updateBrushSample();
	CBrush * createSelectedBrush();

	// Generated message map functions
	//{{AFX_MSG(CBkg)
	afx_msg void OnSelendokHatchbrushes();
	afx_msg void OnSelendokRasterop();
	afx_msg void OnSelendokSolidbrushes();
	afx_msg void OnSelendokStockbrushes();
	afx_msg void OnTransparent();
	afx_msg void OnWinding();
	afx_msg void OnStockbrush();
	afx_msg void OnSolidbrush();
	afx_msg void OnOpaque();
	afx_msg void OnHatchbrush();
	afx_msg void OnAlternate();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowrop();
	afx_msg void OnSelendokBkcolor();
	afx_msg void OnChangeBrushorgx();
	afx_msg void OnChangeBrushorgy();
	afx_msg void OnSelendokWndcolor();
	afx_msg void OnShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
