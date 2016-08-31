// FontMet.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "FontMet.h"
#include "fontobj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CFontObj InfoFontObj("Info", MAKEINTRESOURCE(IDS_FONTINFO), NULL);

/////////////////////////////////////////////////////////////////////////////
// CFontMetrics

CFontMetrics::CFontMetrics()
{
}

CFontMetrics::~CFontMetrics()
{
}


BEGIN_MESSAGE_MAP(CFontMetrics, CTreeCtrl)
        //{{AFX_MSG_MAP(CFontMetrics)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontMetrics message handlers


void CFontMetrics::PreSubclassWindow() 
{
        // TODO: Add your specialized code here and/or call the base class
        
        CTreeCtrl::PreSubclassWindow();

        SetFont(InfoFontObj.GetCurrentFont());
}
