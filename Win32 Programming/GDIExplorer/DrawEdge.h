// DrawEdge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawEdge dialog

class CDrawEdge : public CDialog
{
// Construction
public:
        CDrawEdge(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CDrawEdge)
        enum { IDD = IDD_DRAWEDGE };
        CButton c_Enabled;
        CDrawEdgeButton c_Button;
        CButton c_AutoErase;
        CButton c_DiagonalEndBottomRight;
        CButton c_RaisedInner;
        CButton c_RaisedOuter;
        CButton c_SunkenInner;
        CButton c_SunkenOuter;
        CButton c_Adjust;
        CButton c_Bottom;
        CButton c_BottomLeft;
        CButton c_BottomRight;
        CButton c_Diagonal;
        CButton c_DiagonalEndBottomLeft;
        CButton c_DiagonalEndTopRight;
        CButton c_DiagonalEndTopLeft;
        CButton c_Flat;
        CButton c_Left;
        CButton c_Middle;
        CButton c_Mono;
        CButton c_Rect;
        CButton c_Right;
        CButton c_Soft;
        CButton c_Top;
        CButton c_TopLeft;
        CButton c_TopRight;
        CButton c_EdgeBump;
        CButton c_EdgeEtched;
        CButton c_EdgeRaised;
        CButton c_EdgeSunken;
        CEdgeSample     c_Sample;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDrawEdge)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void setEdge();
        void redraw();
        void setBase();
        void setComposite();
        void eraseSample();
        DWORD getStyles();
        void setHStyle(DWORD newstyle);
        void setVStyle(DWORD newstyle);

        // Generated message map functions
        //{{AFX_MSG(CDrawEdge)
        afx_msg void OnCopy();
        afx_msg void OnBdrRaisedinner();
        afx_msg void OnBdrRaisedouter();
        afx_msg void OnBdrSunkeninner();
        afx_msg void OnBdrSunkenouter();
        afx_msg void OnBfAdjust();
        afx_msg void OnBfBottom();
        afx_msg void OnBfBottomleft();
        afx_msg void OnBfBottomright();
        afx_msg void OnBfDiagonal();
        afx_msg void OnBfDiagonalEndbottomleft();
        afx_msg void OnBfDiagonalEndbottomright();
        afx_msg void OnBfDiagonalEndtopleft();
        afx_msg void OnBfDiagonalEndtopright();
        afx_msg void OnBfFlat();
        afx_msg void OnBfLeft();
        afx_msg void OnBfMiddle();
        afx_msg void OnBfMono();
        afx_msg void OnBfRect();
        afx_msg void OnBfRight();
        afx_msg void OnBfSoft();
        afx_msg void OnBfTop();
        afx_msg void OnBfTopleft();
        afx_msg void OnBfTopright();
        afx_msg void OnEdgeBump();
        afx_msg void OnEdgeEtched();
        afx_msg void OnEdgeRaised();
        afx_msg void OnEdgeSunken();
        virtual BOOL OnInitDialog();
        afx_msg void OnErase();
        afx_msg void OnGetdownstyle();
        afx_msg void OnGetupstyle();
        afx_msg void OnSetdownstyle();
        afx_msg void OnSetupstyle();
        afx_msg void OnB();
        afx_msg void OnHc();
        afx_msg void OnL();
        afx_msg void OnR();
        afx_msg void OnT();
        afx_msg void OnVc();
        afx_msg void OnEnable();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
