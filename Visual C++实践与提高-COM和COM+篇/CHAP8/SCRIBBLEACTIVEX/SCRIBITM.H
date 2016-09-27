// ScribItm.h : interface of the CScribbleItem class
//

#if !defined(AFX_SCRIBITM_H__C4252BF5_DCD0_4FD0_A957_6978356A944D__INCLUDED_)
#define AFX_SCRIBITM_H__C4252BF5_DCD0_4FD0_A957_6978356A944D__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CScribbleItem : public CDocObjectServerItem
{
    DECLARE_DYNAMIC(CScribbleItem)

// Constructors
public:
    CScribbleItem(CScribbleDoc* pContainerDoc);

// Attributes
    CScribbleDoc* GetDocument() const
    {
        return (CScribbleDoc*)COleServerItem::GetDocument();
    }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScribbleItem)
public:
    virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
    virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
    //}}AFX_VIRTUAL

// Implementation
public:
    ~CScribbleItem();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIBITM_H__C4252BF5_DCD0_4FD0_A957_6978356A944D__INCLUDED_)
