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
