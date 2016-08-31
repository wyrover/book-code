// OEMView.cpp : implementation file
//

#include "stdafx.h"
#include "CharacterSets.h"
#include "CharacterSetsDoc.h"
#include "CharacterSetsView.h"
#include "OEMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEMView

IMPLEMENT_DYNCREATE(COEMView, CCharacterSetsView)

COEMView::COEMView()
{
 font.CreateStockObject(OEM_FIXED_FONT);
}

COEMView::~COEMView()
{
}


BEGIN_MESSAGE_MAP(COEMView, CCharacterSetsView)
        //{{AFX_MSG_MAP(COEMView)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEMView diagnostics

#ifdef _DEBUG
void COEMView::AssertValid() const
{
        CCharacterSetsView::AssertValid();
}

void COEMView::Dump(CDumpContext& dc) const
{
        CCharacterSetsView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COEMView message handlers
