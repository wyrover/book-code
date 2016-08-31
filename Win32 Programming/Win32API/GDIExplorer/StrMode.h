// Rop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStretchBltOp window

class CStretchBltOp : public CIDCombo
{
// Construction
public:
        CStretchBltOp();

// Attributes
public:

// Operations
public:
        int Load(int def = 0);
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CStretchBltOp)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CStretchBltOp();

        // Generated message map functions
protected:
        //{{AFX_MSG(CStretchBltOp)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
