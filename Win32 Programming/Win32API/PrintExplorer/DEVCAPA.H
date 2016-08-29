// DevCapa.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceCapabilities dialog

class CDeviceCapabilities : public CPropertyPage
{
	DECLARE_DYNCREATE(CDeviceCapabilities)

// Construction
public:
	CDeviceCapabilities();
	~CDeviceCapabilities();

// Dialog Data
	LPPRINTDLG pd;

	//{{AFX_DATA(CDeviceCapabilities)
	enum { IDD = IDD_DEVCAPABILITIES };
	CPaperNameData	c_DC_PAPERNAMES;
	CPaperSizes	c_DC_PAPERSIZE;
	CPapers		c_DC_PAPERS;
	CStatic	c_c_DC_PAPERS;
	CStatic	c_c_DC_PAPERSIZE;
	CStatic	c_c_DC_PAPERNAMES;
	CSizeCombo	c_DC_FILEDEPENDENCIES;
	CStatic	c_c_DC_FILEDEPENDENCIES;
	CSizeCombo	c_DC_DATATYPE_PRODUCED;
	CStatic	c_c_DC_DATATYPE_PRODUCED;
	CStatic	c_c_DC_BINS;
	CStatic	c_c_DC_BINNAMES;
	CStatic	c_c_DC_ENUMRESOLUTIONS;
	CSizeCombo	c_DC_BINS;
	CSizeCombo	c_DC_BINNAMES;
	CSizeCombo	c_DC_ENUMRESOLUTIONS;
	CStatic	c_c_DC_BINADJUST;
	CButton	c_DC_EMF_COMPLIANT;
	CEnumEdit	c_DC_BINADJUST;
	CNumericEdit	c_DC_FIELDS;
	CHDevNames	c_DEVNAMES;
	CHDevMode	c_DEVMODE;
	CNumericEdit	c_DC_EXTRA;
	CNumericEdit	c_DC_COPIES;
	CNumericEdit	c_DC_DRIVER;
	CButton	c_DC_DUPLEX;
	CMessage	c_Result;
	CNumericEdit	c_DC_SIZE;
	CNumericEdit	c_DC_ORIENTATION;
	CNumericEdit	c_DC_MINEXTENT_Y;
	CNumericEdit	c_DC_MINEXTENT_X;
	CNumericEdit	c_DC_MAXEXTENT_Y;
	CNumericEdit	c_DC_MAXEXTENT_X;
	CNumericEdit	c_DC_VERSION;
	CBitCombo	c_DC_TRUETYPE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDeviceCapabilities)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LPCTSTR device;
	LPCTSTR port;
	LPDEVNAMES dn;
	LPDEVMODE dm;
	DWORD gdc(WORD capa, LPVOID out) {
	    return DeviceCapabilities(device, port, capa, (LPTSTR)out, dm);}

	void dc_to_controls();
	void setEnumResolutions();
	void setBins();
	void setDataTypeProduced();
	void setFileDependencies();
	void setPaperNames();

	// Generated message map functions
	//{{AFX_MSG(CDeviceCapabilities)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
