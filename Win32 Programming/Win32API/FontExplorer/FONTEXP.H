// fontexp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontExplorer dialog

class CFontExplorer : public CFontDialog
{
        DECLARE_DYNAMIC(CFontExplorer)

public:
        CFontExplorer(LPLOGFONT lplfInitial = NULL,
                DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
                CDC* pdcPrinter = NULL,
                CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
        CFontExplorer(const CHARFORMAT& charformat,
                DWORD dwFlags = CF_SCREENFONTS,
                CDC* pdcPrinter = NULL,
                CWnd* pParentWnd = NULL);
#endif

// Dialog Data
        //{{AFX_DATA(CFontExplorer)
        enum { IDD = FORMATDLGORD31 };
        CFontMetrics    c_TreeMetrics;
        CStatic c_c_Legend;
        BOOL debug;
        CLegend c_Legend;
        CStatic c_SmallSample;
        CFontDisplay    c_CharDisplay;
        CButton c_Char;
        CButton c_Info;
        CComboBox       c_Metrics;
        //}}AFX_DATA

public:
        HHOOK hook;  // hook procedure to simulate message map entries due
                     // to improper implementation of MFC. 

        // This ugly hack is due to MFC not being able to find the
        // correct CFontExplorer * object in the map...and no, we don't
        // understand why!
        static CFontExplorer * self;  // grotesque and ugly hack

protected:
        void enableControls();
        void DoDataExchange(CDataExchange* pDX);
        // CDC * getFontDC(CWnd * wnd, CWnd * fontwnd = NULL);
        void updateFontInfo();
        void loadWidths(CFontDescriptor * desc);

        static int LFChanged;
        //{{AFX_MSG(CFontExplorer)
        virtual BOOL OnInitDialog();
        afx_msg void OnInfo();
        afx_msg void OnChar();
        afx_msg void OnOptions();
        afx_msg void OnCopy();
        afx_msg void OnDestroy();
        //}}AFX_MSG
        // afx_msg LRESULT OnFontChanged(WPARAM, LPARAM);
        afx_msg LRESULT OnUpdateFonts(WPARAM, LPARAM);

        DECLARE_MESSAGE_MAP()
};
