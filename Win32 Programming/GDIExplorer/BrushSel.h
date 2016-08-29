// BrushSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrushSelector dialog

class CBrushSelector : public CPropertyPage
{
        DECLARE_DYNCREATE(CBrushSelector)

// Construction
public:
        CBrushSelector();
        ~CBrushSelector();

// Dialog Data
        //{{AFX_DATA(CBrushSelector)
        enum { IDD = IDD_BRUSH };
        CStock  c_StockBrushes;
        CSysColor c_SolidBrushes;
        CBrushStyle c_HatchBrushes;
        CButton c_HatchBrush;
        CButton c_SolidBrush;
        CButton c_StockBrush;
        CIntEdit        c_BrushOrgY;
        CIntEdit        c_BrushOrgX;
        CSpinButtonCtrl c_SpinY;
        CSpinButtonCtrl c_SpinX;
        CStatic c_c_Origin;
        CHatchBrushSample       c_HatchBrushSample;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CBrushSelector)
        public:
        virtual BOOL OnKillActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void updateControls();
        void updateBrushSample();

        // Generated message map functions
        //{{AFX_MSG(CBrushSelector)
        virtual BOOL OnInitDialog();
        afx_msg void OnStockbrush();
        afx_msg void OnSolidbrush();
        afx_msg void OnHatchbrush();
        afx_msg void OnSelendokHatchbrushes();
        afx_msg void OnSelendokSolidbrushes();
        afx_msg void OnSelendokStockbrushes();
        afx_msg void OnChangeBrushorgx();
        afx_msg void OnChangeBrushorgy();
        afx_msg LRESULT OnQuerySiblings(WPARAM, LPARAM lb);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};

