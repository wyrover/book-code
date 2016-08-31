// SynchronizationExplorerDoc.h : interface of the CSynchronizationExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CSynchronizationExplorerDoc : public CDocument
{
protected: // create from serialization only
	CSynchronizationExplorerDoc();
	DECLARE_DYNCREATE(CSynchronizationExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynchronizationExplorerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSynchronizationExplorerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSynchronizationExplorerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
