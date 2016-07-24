// CntrItem.h : interface of the CMultiCastChatCntrItem class
//

#if !defined(AFX_CNTRITEM_H__0F62798F_5A90_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_CNTRITEM_H__0F62798F_5A90_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMultiCastChatDoc;
class CMultiCastChatView;

class CMultiCastChatCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CMultiCastChatCntrItem)

// Constructors
public:
	CMultiCastChatCntrItem(REOBJECT* preo = NULL, CMultiCastChatDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CMultiCastChatDoc* GetDocument()
		{ return (CMultiCastChatDoc*)CRichEditCntrItem::GetDocument(); }
	CMultiCastChatView* GetActiveView()
		{ return (CMultiCastChatView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiCastChatCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CMultiCastChatCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__0F62798F_5A90_11D5_B587_00E04C39B036__INCLUDED_)
