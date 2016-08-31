// OEMView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COEMView view

class COEMView : public CCharacterSetsView
{
protected:
        COEMView();           // protected constructor used by dynamic creation
        DECLARE_DYNCREATE(COEMView)

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(COEMView)
        protected:
        //}}AFX_VIRTUAL

// Implementation
protected:
        virtual ~COEMView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

        // Generated message map functions
protected:
        //{{AFX_MSG(COEMView)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
