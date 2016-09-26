// ContainerActiveXDoc.h : interface of the CContainerActiveXDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTAINERACTIVEXDOC_H__0E34032A_930B_11D4_896A_9349D3D7A679__INCLUDED_)
#define AFX_CONTAINERACTIVEXDOC_H__0E34032A_930B_11D4_896A_9349D3D7A679__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CContainerActiveXDoc : public COleDocument
{
protected: // create from serialization only
    CContainerActiveXDoc();
    DECLARE_DYNCREATE(CContainerActiveXDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContainerActiveXDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CContainerActiveXDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CContainerActiveXDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINERACTIVEXDOC_H__0E34032A_930B_11D4_896A_9349D3D7A679__INCLUDED_)
