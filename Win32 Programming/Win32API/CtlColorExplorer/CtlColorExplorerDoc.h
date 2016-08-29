// CtlColorExplorerDoc.h : interface of the CCtlColorExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CCtlColorExplorerDoc : public CDocument
{
protected: // create from serialization only
        CCtlColorExplorerDoc();
        DECLARE_DYNCREATE(CCtlColorExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCtlColorExplorerDoc)
        public:
        virtual BOOL OnNewDocument();
        virtual void Serialize(CArchive& ar);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CCtlColorExplorerDoc();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        //{{AFX_MSG(CCtlColorExplorerDoc)
                // NOTE - the ClassWizard will add and remove member functions here.
                //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
