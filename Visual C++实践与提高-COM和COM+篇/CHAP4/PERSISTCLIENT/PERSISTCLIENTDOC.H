// PersistClientDoc.h : interface of the CPersistClientDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTCLIENTDOC_H__5D49D00B_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_)
#define AFX_PERSISTCLIENTDOC_H__5D49D00B_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CPersistClientDoc : public CDocument
{
protected: // create from serialization only
    CPersistClientDoc();
    DECLARE_DYNCREATE(CPersistClientDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPersistClientDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CPersistClientDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CPersistClientDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSISTCLIENTDOC_H__5D49D00B_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_)
