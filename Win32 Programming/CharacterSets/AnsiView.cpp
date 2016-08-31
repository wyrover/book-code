// AnsiView.cpp : implementation file
//

#include "stdafx.h"
#include "CharacterSets.h"
#include "CharacterSetsDoc.h"
#include "CharacterSetsView.h"
#include "AnsiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnsiView

IMPLEMENT_DYNCREATE(CAnsiView, CCharacterSetsView)

CAnsiView::CAnsiView()
{
 font.CreateStockObject(ANSI_FIXED_FONT);
}

CAnsiView::~CAnsiView()
{
}


BEGIN_MESSAGE_MAP(CAnsiView, CCharacterSetsView)
        //{{AFX_MSG_MAP(CAnsiView)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnsiView drawing


/////////////////////////////////////////////////////////////////////////////
// CAnsiView diagnostics

#ifdef _DEBUG
void CAnsiView::AssertValid() const
{
        CCharacterSetsView::AssertValid();
}

void CAnsiView::Dump(CDumpContext& dc) const
{
        CCharacterSetsView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnsiView message handlers
