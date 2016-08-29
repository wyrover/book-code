// Pdata.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaperData window
class PaperInfo {
	public:
		 char papername[64];
		 WORD paper;
		 POINT papersize;
		 int id;  
		 PaperInfo();
		 ~PaperInfo();
	       } ;

typedef PaperInfo * LPPaperInfo;


class CPaperData : public CComboBox
{
// Construction
public:
	CPaperData();

// Attributes
public:

// Operations
public:
	int AddString(LPPaperInfo);
	LPPaperInfo GetItemData(int index);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaperData)
	public:
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual LRESULT OnRecomputeWidths(WPARAM, LPARAM);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaperData();

	// Generated message map functions
protected:
	void findPaperID(LPPaperInfo);
	void showPreferredRectangle(CDC *, LPRECT);
	int tabs[20];
	int height;
	int margin;
	virtual void recompute_widths();
	virtual void DrawLine(CDC *, LPPaperInfo, LPDRAWITEMSTRUCT);
	//{{AFX_MSG(CPaperData)
	afx_msg void OnDropdown();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
