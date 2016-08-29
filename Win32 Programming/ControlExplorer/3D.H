// 3D.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3D dialog

typedef struct {
	CStatic	c_WhiteRect;
	CStatic	c_WhiteFrame;
	CStatic	c_GrayRect;
	CStatic	c_GrayFrame;
	CStatic	c_EtchedRect;
	CStatic	c_EtchedFrame;
	CStatic	c_BlackRect;
	CStatic	c_BlackFrame;
	       } ControlSet;

class C3D : public CPropertyPage
{
	DECLARE_DYNCREATE(C3D)

// Construction
public:
	C3D();
	~C3D();

// Dialog Data
	//{{AFX_DATA(C3D)
	enum { IDD = IDD_3D };
	CStatic	c_Etched;
	CButton	c_3D;
	CStatic	c_CopyFrame;
	//}}AFX_DATA

	ControlSet Controls_3D;
	ControlSet Controls_Flat;
#define ControlCount (sizeof(ControlSet) / sizeof(CStatic))
	CStatic * Array3D[ControlCount];
	CStatic * ArrayFlat[ControlCount];

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3D)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void makeEvilTwins(CStatic * flat[], CStatic * c3D[]);
	void evilClone(CStatic * flat, CStatic * c3D);
	void controlsToArray(CStatic * array[], ControlSet * ctls);
	void showWindows(CStatic * array[], BOOL show);

	// Generated message map functions
	//{{AFX_MSG(C3D)
	afx_msg void On3d();
	afx_msg void OnCopy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
