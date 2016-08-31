// Rop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatRop window

class CPatRop : public CIDCombo
{
// Construction
public:
        CPatRop();

// Attributes
public:

// Operations
public:
        int Load(BOOL ext, int def = 0);
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPatRop)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CPatRop();

        // Generated message map functions
protected:
        //{{AFX_MSG(CPatRop)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
