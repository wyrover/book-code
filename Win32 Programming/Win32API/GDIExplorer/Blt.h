// Blt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlt dialog

class CBlt : public CPropertyPage
{
        DECLARE_DYNCREATE(CBlt)

// Construction
public:
        CBlt();  // not really used, necessary for IMPLEMENT_DYNCREATE
        CBlt(int caption);
        ~CBlt();
        CString getRopName(CString &s);

// Dialog Data
        //{{AFX_DATA(CBlt)
        enum { IDD = IDD_BITBLT };
        CBrushDemo c_BrDemo;
        CSource c_Mask;
        CButton c_c_MaskCtl;
        CStatic c_c_SrcX;
        CStatic c_c_SrcY;
        CButton c_Rop_Caption;
        CSpinButtonCtrl c_YMaskSpin;
        CStatic c_c_YMask;
        CIntEdit        c_YMask;
        CSpinButtonCtrl c_XMaskSpin;
        CStatic c_c_XMask;
        CIntEdit        c_XMask;
        CStatic c_c_SourceWidth;
        CStatic c_c_SourceHeight;
        CStatic c_c_pt2y;
        CStatic c_c_pt2x;
        CStatic c_c_pt1y;
        CStatic c_c_pt1x;
        CStatic c_c_pt0y;
        CStatic c_c_pt0x;
        CStatic c_c_Rop;
        CButton c_c_Mask;
        CButton c_ClearMask;
        CButton c_SetMask;
        CButton c_Extended;
        CSpinButtonCtrl c_SrclrYSpin;
        CSpinButtonCtrl c_SrclrXSpin;
        CSpinButtonCtrl c_PlgYSpin;
        CIntEdit        c_PlgY;
        CSpinButtonCtrl c_PlgXSpin;
        CIntEdit        c_PlgX;
        CStatic c_c_Bitmap;
        CIntEdit        c_SrclrY;
        CIntEdit        c_SrclrX;
        CButton c_c_Brush;
        CStatic c_Error;
        CButton c_ClearW;
        CButton c_ClearB;
        CDest   c_Dest;
        CSource c_Source;
        CSpinButtonCtrl c_ulYSpin;
        CSpinButtonCtrl c_ulXSpin;
        CSpinButtonCtrl c_lrYSpin;
        CIntEdit        c_lrY;
        CSpinButtonCtrl c_lrXSpin;
        CIntEdit        c_lrX;
        CSpinButtonCtrl c_SrcYSpin;
        CIntEdit        c_SrcY;
        CSpinButtonCtrl c_SrcXSpin;
        CIntEdit        c_SrcX;
        CIntEdit        c_ulY;
        CIntEdit        c_ulX;
        CButton c_DoBlt;
        CBitmapSelect   c_BitmapSelect;
        CStatic c_Op;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CBlt)
        public:
        virtual BOOL OnSetActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        BOOL isSourceRop(int rop);
        BOOL isPatRop(int rop);

        int lasterror;
        int opid;  // string ID of Blt button
        BOOL initialized;
        virtual void makeOp();
        virtual void updateControls();
        virtual void updateSource();
        virtual BOOL hasSource();
        virtual void showSourceControls(BOOL show);
        virtual void endInitDialog();
        virtual void doBLT();
        virtual void plgShow(BOOL show);
        virtual void maskShow(BOOL show);
        virtual void loadRop(BOOL ext);
        virtual void enableOp();
        virtual CPoint getCopyOrg();
        virtual CSize getCopyExt();

        BOOL createBrush(CBrush * br);
        BOOL queryBrush(LOGBRUSH * lb);
        CSize getSourceBitmapSize();
        virtual void setLastError();

        // Generated message map functions
        //{{AFX_MSG(CBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnChangeSrcx();
        afx_msg void OnChangeSrcy();
        afx_msg void OnChangeLrx();
        afx_msg void OnChangeLry();
        afx_msg void OnChangeUlx();
        afx_msg void OnChangeUly();
        afx_msg void OnDoblt();
        afx_msg void OnClearW();
        afx_msg void OnClearB();
        afx_msg void OnSelendokBitmap();
        afx_msg void OnExtended();
        afx_msg void OnChangeSrclrx();
        afx_msg void OnChangeSrclry();
        afx_msg void OnCopy();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
