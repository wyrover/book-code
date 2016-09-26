// CntrItem.h : interface of the CContainerActiveXCntrItem class
//

#if !defined(AFX_CNTRITEM_H__0E34032E_930B_11D4_896A_9349D3D7A679__INCLUDED_)
#define AFX_CNTRITEM_H__0E34032E_930B_11D4_896A_9349D3D7A679__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CContainerActiveXDoc;
class CContainerActiveXView;

class CContainerActiveXCntrItem : public COleDocObjectItem
{
    DECLARE_SERIAL(CContainerActiveXCntrItem)

// Constructors
public:
    CContainerActiveXCntrItem(CContainerActiveXDoc* pContainer = NULL);
    // Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
    //  IMPLEMENT_SERIALIZE requires the class have a constructor with
    //  zero arguments.  Normally, OLE items are constructed with a
    //  non-NULL document pointer.

// Attributes
public:
    CContainerActiveXDoc* GetDocument()
    {
        return (CContainerActiveXDoc*)COleDocObjectItem::GetDocument();
    }
    CContainerActiveXView* GetActiveView()
    {
        return (CContainerActiveXView*)COleDocObjectItem::GetActiveView();
    }

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContainerActiveXCntrItem)
public:
    virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
    virtual void OnActivate();
protected:
    virtual void OnDeactivateUI(BOOL bUndoable);
    virtual BOOL OnChangeItemPosition(const CRect& rectPos);
    //}}AFX_VIRTUAL

// Implementation
public:
    ~CContainerActiveXCntrItem();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__0E34032E_930B_11D4_896A_9349D3D7A679__INCLUDED_)
