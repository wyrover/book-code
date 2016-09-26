// CntrItem.h : interface of the CContainerItem class
//

class CContainerDoc;
class CContainerView;

class CContainerItem : public COleClientItem
{
    DECLARE_SERIAL(CContainerItem)

// Constructors
public:
    CContainerItem(CContainerDoc* pContainer = NULL);
    // Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
    //  IMPLEMENT_SERIALIZE requires the class have a constructor with
    //  zero arguments.  Normally, OLE items are constructed with a
    //  non-NULL document pointer.

// Attributes
public:
    CRect m_rect;   // position within the document
    CContainerDoc* GetDocument()
    {
        return (CContainerDoc*)COleClientItem::GetDocument();
    }
    CContainerView* GetActiveView()
    {
        return (CContainerView*)COleClientItem::GetActiveView();
    }

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContainerItem)
public:
    virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
protected:
    virtual void OnGetItemPosition(CRect& rPosition);
    virtual void OnActivate();
    virtual void OnDeactivateUI(BOOL bUndoable);
    virtual BOOL OnChangeItemPosition(const CRect& rectPos);
    //}}AFX_VIRTUAL

// Implementation
public:
    ~CContainerItem();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////
