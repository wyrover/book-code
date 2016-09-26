// ScribDoc.h : interface of the CScribbleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIBDOC_H__134561FF_CBDF_475C_A719_FF8CE81F10BA__INCLUDED_)
#define AFX_SCRIBDOC_H__134561FF_CBDF_475C_A719_FF8CE81F10BA__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CScribbleItem;

class CScribbleDoc : public COleServerDoc
{
protected: // create from serialization only
    CScribbleDoc();
    DECLARE_DYNCREATE(CScribbleDoc)

// Attributes
public:
    CScribbleItem* GetEmbeddedItem()
    {
        return (CScribbleItem*)COleServerDoc::GetEmbeddedItem();
    }

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScribbleDoc)
protected:
    virtual COleServerItem* OnGetEmbeddedItem();
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CScribbleDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CScribbleDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIBDOC_H__134561FF_CBDF_475C_A719_FF8CE81F10BA__INCLUDED_)
