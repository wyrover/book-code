// select.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelect dialog

class CSelect : public CDialog
{
// Construction
public:
        CSelect(CWnd* pParent = NULL);   // standard constructor
        int style;
        RandomData rd;
        

// Dialog Data
        //{{AFX_DATA(CSelect)
        enum { IDD = IDD_EXAMPLE };
        CButton c_AngleArc;
        CButton c_GeoPen;
        CButton c_TextXForm;
        CButton c_XForm;
        CButton c_ArcTo;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSelect)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(CSelect)
        afx_msg void OnBezier();
        afx_msg void OnEndcaps();
        afx_msg void OnLinejoin();
        afx_msg void OnTextClip1();
        virtual BOOL OnInitDialog();
        afx_msg void OnXform();
        afx_msg void OnTextxform();
        afx_msg void OnAnglearc();
        afx_msg void OnArc();
        afx_msg void OnArcTo();
        afx_msg void OnEllipse();
        afx_msg void OnChord();
        afx_msg void OnPie();
        afx_msg void OnRectangle();
        afx_msg void OnRoundrect();
        afx_msg void OnPenstyles();
        afx_msg void OnPolygon();
        afx_msg void OnDrawfocus();
        afx_msg void OnHatchbrush();
        afx_msg void OnStrokeandfill();
        afx_msg void OnTextclip2();
        afx_msg void OnTextclip3();
        afx_msg void OnComplexPath();
        afx_msg void OnRandom();
        afx_msg void OnBmpbrush();
        afx_msg void OnGeopen();
        afx_msg void OnEndcapsStroked();
        afx_msg void OnLinejoinStroked();
        afx_msg void OnPolypolygon();
        //}}AFX_MSG
        BOOL Is32s();
        DECLARE_MESSAGE_MAP()
};
