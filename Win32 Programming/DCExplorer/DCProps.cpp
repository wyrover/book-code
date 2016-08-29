// DCProps.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "DCProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCProperties

IMPLEMENT_DYNAMIC(CDCProperties, CPropertySheet)

CDCProperties::CDCProperties(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
 page = 0;
}

CDCProperties::CDCProperties(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CDCProperties::~CDCProperties()
{
}


BEGIN_MESSAGE_MAP(CDCProperties, CPropertySheet)
	//{{AFX_MSG_MAP(CDCProperties)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCProperties message handlers

BOOL CDCProperties::OnInitDialog() 
{
 	SetActivePage(page);
	
	return CPropertySheet::OnInitDialog();
}
