// lfexp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogFontExplorer dialog

class CLogFontExplorer : public CDialog
{
// Construction
public:
        CLogFontExplorer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CLogFontExplorer)
        enum { IDD = IDD_LOGFONT };
        CFontMetrics    c_Data;
        CFontName       c_lfFaceName;
        CSample c_Sample;
        CMessage        c_Result;
        CEdit   c_lfWidth;
        CEdit   c_lfHeight;
        CDegrees        c_lfEscapement;
        CDegrees        c_lfOrientation;
        CRotation       c_SpinOrientation;
        CRotation       c_SpinEscapement;
        CStatic c_c_lfOrientation;
        CButton c_GM_ADVANCED;
        CIDCombo        c_lfFamily;
        CIDCombo        c_lfPitch;
        CIDCombo        c_lfWeight;
        CIDCombo        c_lfQuality;
        CIDCombo        c_lfOutPrecision;
        CIDCombo        c_lfCharSet;
        CIDCombo        c_lfClipPrecision;
        CButton c_lfUnderline;
        CButton c_lfItalic;
        CButton c_lfStrikeOut;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CLogFontExplorer)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        LOGFONT lf;
        HFONT font;

        void setMessage(DWORD result);
        BOOL Is95();
        void controls_to_lf();

        // Generated message map functions
        //{{AFX_MSG(CLogFontExplorer)
        virtual BOOL OnInitDialog();
        afx_msg void OnDestroy();
        afx_msg void OnCreateLogFont();
        afx_msg void OnSelendokLffamily();
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnGmAdvanced();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
