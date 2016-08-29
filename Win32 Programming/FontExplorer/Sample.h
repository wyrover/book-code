// Sample.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSample window

class CSample : public CStatic
{
// Construction
public:
        CSample();
        int mode;

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSample)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CSample();

        // Generated message map functions
protected:
        //{{AFX_MSG(CSample)
        afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
