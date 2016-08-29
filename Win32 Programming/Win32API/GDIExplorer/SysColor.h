// SysColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysColor window

class CSysColor : public CColor
{
// Construction
public:
        CSysColor();

// Attributes
public:

// Operations
public:
        int Load(int sel = 0);

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSysColor)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CSysColor();

        // Generated message map functions
protected:
        //{{AFX_MSG(CSysColor)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
