// OleClipDoc.h : interface of the COleClipDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLECLIPDOC_H__C4CDBECE_616B_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_OLECLIPDOC_H__C4CDBECE_616B_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class COleClipDoc : public CDocument
{
protected: // create from serialization only
    COleClipDoc();
    DECLARE_DYNCREATE(COleClipDoc)

// Attributes
public:
    CDib m_dib;
    BOOL m_bDragHere; // for drags between two wins linked to this doc
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COleClipDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    void DeleteContents();
    virtual ~COleClipDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
public:
    //{{AFX_MSG(COleClipDoc)
    afx_msg void OnEditClearAll();
    afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLECLIPDOC_H__C4CDBECE_616B_11D4_A23D_02608CACEDBB__INCLUDED_)
