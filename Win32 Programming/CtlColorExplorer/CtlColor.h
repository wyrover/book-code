// CtlColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCtlColorParms dialog

class CCtlColorParms : public CPropertyPage
{
        DECLARE_DYNCREATE(CCtlColorParms)

// Construction
public:
        CCtlColorParms(UINT caption = IDD, UINT msg = 0);
        ~CCtlColorParms();

// Dialog Data
        //{{AFX_DATA(CCtlColorParms)
        enum { IDD = IDD_CTLCOLOR };
        CStatic c_TextColorCaption;
        CStatic c_BkColorCaption;
        CStatic c_BkBrushCaption;
        CButton c_FontGroup;
        CButton c_ChangeFont;
        CStatic c_Sample;
        CButton c_ClearFont;
        CButton c_NoSetFont;
        CStatic c_FaceName;
        CRGB    c_RGBText;
        CRGB    c_RGBBrush;
        CRGB    c_RGBBkg;
        CColor  c_BkBrush;
        CColor  c_TextColor;
        CColor  c_BkColor;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CCtlColorParms)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        UINT msgid;
        CBrush brush;
        CFont font;
        CString getWeightString(int weight);
        LRESULT SetDC(UINT type, CDC * dc);
        LRESULT SetFont(UINT type, CWnd * wnd);
        void enableFontOptions();
        void enableColorOptions();

        // Generated message map functions
        //{{AFX_MSG(CCtlColorParms)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokBkbrush();
        afx_msg void OnSelendokBkcolor();
        afx_msg void OnSelendokTextcolor();
        afx_msg void OnChangefont();
        afx_msg void OnNosetfont();
        afx_msg void OnClearfont();
        afx_msg void OnDestroy();
        afx_msg void OnTransparent();
        afx_msg void OnOpaque();
        //}}AFX_MSG
        afx_msg LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()

};
