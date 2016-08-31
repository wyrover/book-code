// HeapExplorerDoc.h : interface of the CHeapExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CHeapExplorerDoc : public CDocument
{
protected: // create from serialization only
        CHeapExplorerDoc();
        DECLARE_DYNCREATE(CHeapExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CHeapExplorerDoc)
        public:
        virtual BOOL OnNewDocument();
        virtual void Serialize(CArchive& ar);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CHeapExplorerDoc();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        //{{AFX_MSG(CHeapExplorerDoc)
                // NOTE - the ClassWizard will add and remove member functions here.
                //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
