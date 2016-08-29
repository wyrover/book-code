// FontDisp.h : header file
//
typedef struct {int win; int view; } iscale;


/////////////////////////////////////////////////////////////////////////////
// CFontDisplay window

typedef struct {
        TCHAR ch;
        int pos;
               } PosInfo;

class CFontDisplay : public CStatic
{
// Construction
public:
        CFontDisplay(BOOL dokerning = FALSE);
        display_options opts;
        COLORREF color;
// Attributes
public:
// Operations
public:
        static LegendInfo ** GetLegend();
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFontDisplay)
        protected:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CFontDisplay();

        // Generated message map functions
protected:
#ifdef _DEBUG
        PosInfo * pinfo;
#endif
        CFont legendFont;
        int GetDisplayCode(CDC & dc, TCHAR ch);
        static inline int f(int x) { return (x == 0 ? 0 : 
                                        x < 0 ? 1 : 2); }
        int requiredHeight(CFontDescriptor & desc);
        static float limitScaling(TEXTMETRIC & tm, float scale);

        void drawABCs(CDC & dc, int pos, TCHAR ch, int top, int bottom, int parity);
        void drawDistance(CDC & dc, int from, int to, int ypos, LPCTSTR legend, int ylegend);
        void drawV(CDC & dc, int x, int top, int bottom);
        void writeRulerV(CDC & dc, int left, int top, int bottom);
        void writeRulerH(CDC & dc, int pos, int left, int right);
        CPopupList * popup;
        BOOL dismissPopup(CPoint point);
        SIZE getOverhangTextExtent(CDC & dc, CString s);
        iscale normalizeScaling(float sx);
        //{{AFX_MSG(CFontDisplay)
        afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg UINT OnNcHitTest(CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnDestroy();
        afx_msg void OnCancelMode();
        afx_msg void OnSize(UINT nType, int cx, int cy);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
