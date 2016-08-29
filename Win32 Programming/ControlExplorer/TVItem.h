class CTV_ITEM : public CTreeOps 
    {
     DECLARE_DYNCREATE(CTV_ITEM)
  public:
     static TV_ITEM item;
     static TCHAR text[256];
     CTV_ITEM();
     CTV_ITEM(int idd);
     virtual ~CTV_ITEM();
     void controlsToItem(LPTV_ITEM item = NULL, BOOL forceTVIF=FALSE);
     void itemToControls(LPTV_ITEM item = NULL);
// Dialog Data
	//{{AFX_DATA(CTV_ITEM)
	CButton	c_CheckAll;
	CButton	c_c_State_StateMask;
	CButton	c_iImage_Callback;
	CButton	c_iSelectedImage_Callback;
	CButton	c_m_TVIS_BOLD;
	CButton	c_m_TVIS_CUT;
	CButton	c_m_TVIS_DROPHILITED;
	CButton	c_m_TVIS_EXPANDED;
	CButton	c_m_TVIS_EXPANDEDONCE;
	CButton	c_m_TVIS_FOCUSED;
	CButton	c_m_TVIS_OVERLAYMASK;
	CButton	c_m_TVIS_SELECTED;
	CButton	c_m_TVIS_STATEIMAGEMASK;
	CButton	c_TVIF_CHILDREN;
	CButton	c_TVIF_HANDLE;
	CButton	c_TVIF_IMAGE;
	CButton	c_TVIF_PARAM;
	CButton	c_TVIF_SELECTEDIMAGE;
	CButton	c_TVIF_STATE;
	CButton	c_TVIF_TEXT;
	CButton	c_TVIS_BOLD;
	CButton	c_TVIS_CUT;
	CButton	c_TVIS_DROPHILITED;
	CButton	c_TVIS_EXPANDED;
	CButton	c_TVIS_EXPANDEDONCE;
	CButton	c_TVIS_FOCUSED;
	CButton	c_TVIS_SELECTED;
	CButton c_pszText_Callback;
	CComboBox	c_cChildren;
	CComboBox	c_hItem;
	CEdit	c_pszText;
	CNumericEdit	c_iImage;
	CNumericEdit	c_iSelectedImage;
	CNumericEdit	c_lParam;
	CNumericEdit	c_Overlay;
	CNumericEdit	c_StateImage;
	CSpinButtonCtrl	c_SpinOverlay;
	CSpinButtonCtrl	c_SpinStateImage;
	CSpinButtonCtrl	c_Spin_iImage;
	CSpinButtonCtrl	c_Spin_iSelectedImage;
	CSpinButtonCtrl	c_Spin_lParam;
	CStatic	c_c_cChildren;
	CStatic	c_c_hItem;
	CStatic	c_c_iImage;
	CStatic	c_c_iSelectedImage;
	CStatic	c_c_lParam;
	CStatic	c_c_pszText;
	CStatic	c_c_state;
	CStatic	c_c_stateMask;
	CStatic	c_TVIS_OVERLAYMASK;
	CStatic	c_TVIS_STATEIMAGEMASK;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTree)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void enableControls();

// Implementation
protected:
	int selectChildren(int code);

	// Generated message map functions
	//{{AFX_MSG(CTV_ITEM)
	afx_msg void OnCheckall();
	virtual BOOL OnInitDialog();
	afx_msg void OnTextCallback();
	afx_msg void OnIimageCallback();
	afx_msg void OnIselectedimageCallback();
	afx_msg void OnMaskTvisBold();
	afx_msg void OnMaskTvisCut();
	afx_msg void OnMaskTvisDrophilited();
	afx_msg void OnMaskTvisExpanded();
	afx_msg void OnMaskTvisExpandedonce();
	afx_msg void OnMaskTvisFocused();
	afx_msg void OnMaskTvisOverlaymask();
	afx_msg void OnMaskTvisSelected();
	afx_msg void OnMaskTvisStateimagemask();
	afx_msg void OnTvifChildren();
	afx_msg void OnTvifHandle();
	afx_msg void OnTvifImage();
	afx_msg void OnTvifParam();
	afx_msg void OnTvifSelectedimage();
	afx_msg void OnTvifState();
	afx_msg void OnTvifText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    };
