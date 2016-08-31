// Delta.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDelta window

class CDelta : public CStatic
{
// Construction
public:
        CDelta();

// Attributes
public:
        int actual;
        int loops;
// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDelta)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CDelta();

        // Generated message map functions
protected:
        //{{AFX_MSG(CDelta)
        afx_msg void OnPaint();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
