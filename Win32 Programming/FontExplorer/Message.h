// Message.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessage window

class CMessage : public CEdit
{
// Construction
public:
        CMessage();

// Attributes
public:

// Operations
public:
        HBRUSH OnCtlColor(CDC * pdc);
        void SetWindowText(LPCSTR text);

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMessage)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CMessage();

        // Generated message map functions
protected:
        //{{AFX_MSG(CMessage)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
