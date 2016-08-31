// RecEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "RecEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecreatableEdit

CRecreatableEdit::CRecreatableEdit()
{
}

CRecreatableEdit::~CRecreatableEdit()
{
}


BEGIN_MESSAGE_MAP(CRecreatableEdit, CEdit)
        //{{AFX_MSG_MAP(CRecreatableEdit)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecreatableEdit message handlers

void CRecreatableEdit::PostNcDestroy() 
{
}
