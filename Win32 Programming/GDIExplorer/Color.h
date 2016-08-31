// Color.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColor window


class CColor : public CGraphicCombo
{
// Construction
public:
        CColor();
        int Load(int sel = 0);
// Attributes
public:

// Operations
public:

// Overrides
        virtual void drawSample(LPRECT r, LPDRAWITEMSTRUCT dis);
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CColor)
        public:
        //}}AFX_VIRTUAL
#if 0
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
#endif

// Implementation
public:
        virtual ~CColor();

        // Generated message map functions
protected:
        //{{AFX_MSG(CColor)
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
