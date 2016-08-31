// DevMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDevMode dialog

class CDevMode : public CPropertyPage
{
	DECLARE_DYNCREATE(CDevMode)

// Construction
public:
	CDevMode();
	~CDevMode();
	PRINTDLG * pd;

// Dialog Data
	//{{AFX_DATA(CDevMode)
	enum { IDD = IDD_DEVMODE };
	CMessage	c_Result;
	CStatic	c_c_dmSpecVersion;
	CStatic	c_c_dmSize;
	CStatic	c_c_dmFields;
	CStatic	c_c_dmDriverVersion;
	CStatic	c_c_dmDriverExtra;
	CStatic	c_c_dmDeviceName;
	CEnumEdit	c_dmPaperSize;
	CNumericEdit	c_dmYResolution;
	CStatic	c_c_dmYResolution;
	CNumericEdit	c_dmDefaultSource;
	CStatic	c_c_dmCollate;
	CStatic	c_c_dmPrintQuality;
	CEnumEdit	c_dmPrintQuality;
	CEnumEdit	c_dmTTOption;
	CEnumEdit	c_dmDisplayFrequency;
	// CEnumEdit	c_dmDisplayFlags;
	CEnumEdit	c_dmCollate;
	CNumericEdit	c_dmPelsWidth;
	CEnumEdit	c_dmDuplex;
	CNumericEdit	c_dmPelsHeight;
	CNumericEdit	c_dmLogPixels;
	CEdit	c_dmFormName;
	CNumericEdit	c_dmBitsPerPel;
	CStatic	c_c_dmTTOption;
	CStatic	c_c_dmPelsWidth;
	CStatic	c_c_dmPelsHeight;
	CStatic	c_c_dmDisplayFrequency;
	// CStatic	c_c_dmDisplayFlags;
	CStatic	c_c_dmBitsPerPel;
	CStatic	c_c_dmDuplex;
	CStatic	c_c_dmLogPixels;
	CStatic	c_c_dmFormName;
	CEnumEdit	c_dmColor;
	CNumericEdit	c_dmScale;
	CNumericEdit	c_dmPaperWidth;
	CNumericEdit	c_dmPaperLength;
	CNumericEdit	c_dmCopies;
	CNumericEdit	c_dmSpecVersion;
	CNumericEdit	c_dmSize;
	CEnumEdit	c_dmOrientation;
	CNumericEdit	c_dmFields;
	CNumericEdit	c_dmDriverVersion;
	CNumericEdit	c_dmDriverExtra;
	CEdit	c_dmDeviceName;
	CStatic	c_c_dmScale;
	CStatic	c_c_dmPaperWidth;
	CStatic	c_c_dmPaperSize;
	CStatic	c_c_dmPaperLength;
	CStatic	c_c_dmOrientation;
	CStatic	c_c_dmDefaultSource;
	CStatic	c_c_dmCopies;
	CStatic	c_c_dmColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDevMode)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void pd_to_controls();
	void ShowFields(LPDEVMODE dm = NULL, DWORD Fields = 0);
	// Generated message map functions
	//{{AFX_MSG(CDevMode)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
