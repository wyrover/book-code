// IntEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntEdit window

class CIntEdit : public CEdit
{
// Construction
public:
        CIntEdit();
        int GetVal();
        void SetVal(int f);
        BOOL IsBlank();
        BOOL GetReadOnly();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CIntEdit)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CIntEdit();

        // Generated message map functions
protected:
        //{{AFX_MSG(CIntEdit)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
