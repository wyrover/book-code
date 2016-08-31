// Stock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStock window

class CStock : public CGraphicCombo
{
// Construction
public:
        CStock();

// Attributes
public:

// Operations
public:
        void Load(int sel = 0);

// Overrides
        virtual void drawSample(LPRECT r, LPDRAWITEMSTRUCT dis);
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CStock)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CStock();

        // Generated message map functions
protected:
        //{{AFX_MSG(CStock)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
