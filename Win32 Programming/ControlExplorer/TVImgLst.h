// TVImgLst.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTVImageList dialog

class CTVImageList : public CTreeOps
{
	DECLARE_DYNCREATE(CTVImageList)

// Construction
public:
	CTVImageList();
	~CTVImageList();

// Dialog Data
	//{{AFX_DATA(CTVImageList)
	enum { IDD = IDD_TREEIMAGES };
	CSpinButtonCtrl	c_SpinOverlaySource;
	CTinyButton	c_SetOverlayImage;
	CNumericEdit	c_OverlaySource;
	CButton	c_c_OverlayMask;
	CButton	c_OV4;
	CButton	c_OV3;
	CButton	c_OV2;
	CButton	c_OV1;
	CComboBox	c_ImageLists;
	CTinyButton	c_SetImageListNull;
	CButton	c_TVSIL_STATE;
	CButton	c_TVSIL_NORMAL;
	CTinyButton	c_SetImageList;
	CListCtrl	c_Images;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTVImageList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableControls();
	void setimage(HIMAGELIST img);
	// Generated message map functions
	//{{AFX_MSG(CTVImageList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetimagelist();
	afx_msg void OnSetimagelistnull();
	afx_msg void OnSelendokImagelists();
	afx_msg void OnSetoverlayimage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
