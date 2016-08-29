// pCombo.cpp : implementation file
//

#include "stdafx.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "pCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteCombo

CPaletteCombo::CPaletteCombo()
{
}

CPaletteCombo::~CPaletteCombo()
{
}


BEGIN_MESSAGE_MAP(CPaletteCombo, CColor)
	//{{AFX_MSG_MAP(CPaletteCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteCombo message handlers

/****************************************************************************
*                          CPaletteCombo::AddString
* Inputs:
*       int i: Index of palette entry
*	LPPALETTEENTRY pe: Palette entry
* Result: int
*       index of insertion
* Effect: 
*       Adds a formatted palette entry
****************************************************************************/

int CPaletteCombo::AddString(int i, LPPALETTEENTRY pe)
    {
     CString s;

     s.Format(_T("%03d: RGB(%03d, %03d, %03d)"), i, pe->peRed, 
     						    pe->peGreen, 
						    pe->peBlue);

     int index = CColor::AddString(s);

     if(index == CB_ERR)
	return CB_ERR;

     COLORREF c = RGB(pe->peRed, pe->peGreen, pe->peBlue);

     SetItemData(index, c);

     return index;
    }

/****************************************************************************
*                             CPaletteCombo::Load
* Inputs:
*       CPalette * p: Palette to load (default NULL)
* Result: int
*       0, always
* Effect: 
*       Loads the palette data into the combo.  If the palette passed in is
*	NULL, uses the DEFAULT_PALETTE
****************************************************************************/

int CPaletteCombo::Load(CPalette * p)
    {
     PALETTEENTRY pe[256];
     CPalette dp;

     if(p == NULL)
        { /* use default */
	 dp.CreateStockObject(DEFAULT_PALETTE);
	 p = &dp;
	} /* use default */

     int n = p->GetPaletteEntries(0, 256, pe);
     if(n == 0)
	{ /* failed */
	 return CB_ERR;
	} /* failed */

     for(int i = 0; i < n; i++)
	AddString(i, &pe[i]);
     
     return 0;
    }
