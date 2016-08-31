// BitRop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitRop window

class CBitRop : public CIDCombo
{
// Construction
public:
        CBitRop();

// Attributes
public:

// Operations
public:
        int Load(BOOL ext, int def = 0);
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CBitRop)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CBitRop();

        // Generated message map functions
protected:
        //{{AFX_MSG(CBitRop)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
