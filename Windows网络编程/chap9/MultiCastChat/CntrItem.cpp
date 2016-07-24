// CntrItem.cpp : implementation of the CMultiCastChatCntrItem class
//

#include "stdafx.h"
#include "MultiCastChat.h"

#include "MultiCastChatDoc.h"
#include "MultiCastChatView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatCntrItem implementation

IMPLEMENT_SERIAL(CMultiCastChatCntrItem, CRichEditCntrItem, 0)

CMultiCastChatCntrItem::CMultiCastChatCntrItem(REOBJECT* preo, CMultiCastChatDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
	
}

CMultiCastChatCntrItem::~CMultiCastChatCntrItem()
{
	// TODO: add cleanup code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatCntrItem diagnostics

#ifdef _DEBUG
void CMultiCastChatCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CMultiCastChatCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
