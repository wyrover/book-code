// XForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXForm dialog

class CXForm : public CPropertyPage
{
	DECLARE_DYNCREATE(CXForm)

// Construction
public:
	CXForm();
	~CXForm();
	CDCValues * dcv;

// Dialog Data
	//{{AFX_DATA(CXForm)
	enum { IDD = IDD_XFORM };
	CButton	c_ATan;
	CButton	c_Tan;
	CButton	c_2Tan;
	CButton	c_VShear;
	CStatic	c_Operations;
	CButton	c_HShear;
	CButton	c_Rotate;
	CSpinButtonCtrl	c_SpinDY;
	CSpinButtonCtrl	c_SpinDX;
	CSpinButtonCtrl	c_Spin22;
	CSpinButtonCtrl	c_Spin21;
	CSpinButtonCtrl	c_Spin12;
	CSpinButtonCtrl	c_Spin11;
	CButton	c_Bottom;
	CButton	c_Top;
	CButton	c_BaseLine;
	CSystemMessage	c_Error;
	CStatic	c_c_Degrees;
	CButton	c_Advanced;
	CButton	c_c_Xform;
	CButton	c_c_Trig;
	CMatrixText	c_TextSample;
	CButton	c_Identity;
	CMatrixGraphic	c_GraphicSample;
	CButton	c_Compatible;
	CFloatEdit	c_dY;
	CFloatEdit	c_dX;
	CFloatEdit	c_M22;
	CFloatEdit	c_M21;
	CFloatEdit	c_M12;
	CFloatEdit	c_M11;
	CButton	c_ASin;
	CButton	c_ACos;
	CButton	c_Sin;
	CButton	c_Cos;
	CFloatEdit	c_Value;
	CFloatEdit	c_Degrees;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CXForm)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int getAlign();

	void enableControls();
	void matrixToControls(XFORM * matrix);
	void controlsToMatrix(XFORM * matrix);
	void updateSamples();
	LRESULT OnUpdateError(WPARAM wParam, LPARAM);

	// Generated message map functions
	//{{AFX_MSG(CXForm)
	afx_msg void OnAcos();
	afx_msg void OnAsin();
	afx_msg void OnCos();
	afx_msg void OnSin();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDegrees();
	afx_msg void OnChangeValue();
	afx_msg void OnIdentity();
	afx_msg void OnChangeDx();
	afx_msg void OnChangeDy();
	afx_msg void OnChangeM11();
	afx_msg void OnChangeM12();
	afx_msg void OnChangeM21();
	afx_msg void OnChangeM22();
	afx_msg void OnAdvanced();
	afx_msg void OnCompatible();
	afx_msg void OnBaseline();
	afx_msg void OnBottom();
	afx_msg void OnTop();
	afx_msg void OnRotate();
	afx_msg void OnHshear();
	afx_msg void OnVshear();
	afx_msg void OnTan();
	afx_msg void On2tan();
	afx_msg void OnAtan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
