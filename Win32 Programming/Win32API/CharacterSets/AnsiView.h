// AnsiView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnsiView view

class CAnsiView : public CCharacterSetsView
{
protected:
        CAnsiView();           // protected constructor used by dynamic creation
        DECLARE_DYNCREATE(CAnsiView)

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CAnsiView)
        protected:
        //}}AFX_VIRTUAL

// Implementation
protected:
        virtual ~CAnsiView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

        // Generated message map functions
protected:
        //{{AFX_MSG(CAnsiView)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
