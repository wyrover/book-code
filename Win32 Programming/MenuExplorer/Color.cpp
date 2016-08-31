// Color.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "idcombo.h"
#include "gcombo.h"
#include "Color.h"
#include "colors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct { UINT str; COLORREF color; } ColorTable;

static IDData colors[] = {                      // Default palette index
        { IDS_BLACK,    RGB(  0,   0,   0)},    // 000
        { IDS_MAROON,   RGB(128,   0,   0)},    // 001
        { IDS_GREEN,    RGB(  0, 128,   0)},    // 002
        { IDS_OLIVE,    RGB(128, 128,   0)},    // 003
        { IDS_NAVY,     RGB(  0,   0, 128)},    // 004
        { IDS_PURPLE,   RGB(128,   0, 128)},    // 005
        { IDS_TEAL,     RGB(  0, 128, 128)},    // 006
        { IDS_LTGRAY,   RGB(192, 192, 192)},    // 007 // aka Silver
        { IDS_PALEGREEN,RGB(192, 220, 192)},    // 008
        { IDS_SKYBLUE,  RGB(166, 202, 240)},    // 009
        { IDS_CREAM,    RGB(255, 251, 240)},    // 010
        { IDS_MDGRAY,   RGB(160, 160, 164)},    // 011
        { IDS_GRAY,     RGB(128, 128, 128)},    // 012
        { IDS_RED,      RGB(255,   0,   0)},    // 013
        { IDS_LIME,     RGB(0,   255,   0)},    // 014
        { IDS_YELLOW,   RGB(255, 255,   0)},    // 015
        { IDS_BLUE,     RGB(  0,   0, 255)},    // 016
        { IDS_MAGENTA,  RGB(255,   0, 255)},    // 017 // aka Fuschia
        { IDS_CYAN,     RGB(  0, 255, 255)},    // 018 // aka Aqua
        { IDS_WHITE,    RGB(255, 255, 255)},    // 019
        { IDS_DKGRAY,   RGB( 63,  63,  63)},    // --- // not in palette
        { 0,            RGB(  0,   0,   0)}     // EOT // value not used
                      };


/////////////////////////////////////////////////////////////////////////////
// CColor

CColor::CColor()
{
}

CColor::~CColor()
{
}


BEGIN_MESSAGE_MAP(CColor, CGraphicCombo)
        //{{AFX_MSG_MAP(CColor)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColor message handlers

/****************************************************************************
*                             CColor::Load
* Inputs:
*       int sel: Selection to make after loading, the ID of the selection
* Result: int
*       Index of selection, or CB_ERR if no selection made
* Effect: 
*       Loads the control with the colors from the color table
****************************************************************************/

int CColor::Load(int sel)
    {
     return AddStrings(colors, sel);
    }

/****************************************************************************
*                             CColor::drawSample
* Inputs:
*       LPRECT r0: Rectangle to draw sample in
*       LPDRAWITEMSTRUCT dis: Owner-draw structure
* Result: void
*       
* Effect: 
*       Draws the color sample in the indicated rectangle
****************************************************************************/

void CColor::drawSample(LPRECT r0, LPDRAWITEMSTRUCT dis)
    {
     CDC * dc = CDC::FromHandle(dis->hDC);
     CBrush br;
     CBrush * oldbrush;
     CRect r = *r0;
     
     br.CreateSolidBrush((COLORREF)dis->itemData);
     oldbrush = dc->SelectObject(&br);

     r.InflateRect(-1, -1);

     dc->Rectangle(&r);
     dc->SelectObject(oldbrush);
     br.DeleteObject();
    }


#if 0
void CColor::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 CRect r = dis->rcItem;
 SIZE w;
 CDC * dc = CDC::FromHandle(dis->hDC);
 CBrush br;
 CBrush * oldbrush;
 COLORREF txcolor;
 COLORREF bkcolor;

 if(dis->itemState & ODS_SELECTED)
    { /* selected */
     bkcolor = GetSysColor(COLOR_HIGHLIGHT);
     txcolor = GetSysColor(COLOR_HIGHLIGHTTEXT);
    } /* selected */
 else
    { /* unselected */
     if(dis->itemState & (ODS_DISABLED | ODS_GRAYED))
        txcolor = GetSysColor(COLOR_GRAYTEXT);
     else
        txcolor = GetSysColor(COLOR_WINDOWTEXT);

     bkcolor = GetSysColor(COLOR_WINDOW);
    } /* unselected */

 dc->SetBkColor(bkcolor);
 dc->SetTextColor(txcolor);

 br.CreateSolidBrush(bkcolor);
 oldbrush = dc->SelectObject(&br);

 dc->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);

 dc->SelectObject(oldbrush);
 br.DeleteObject();

 if(dis->itemID != (UINT)-1 
    && (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) == 0)
    { /* has item */
     
     w = dc->GetTextExtent(_T("WW"), 2);

     r.right = r.left + w.cx;

     br.CreateSolidBrush((COLORREF)dis->itemData);
     oldbrush = dc->SelectObject(&br);

     r.InflateRect(-1, -1);

     dc->Rectangle(&r);
     dc->SelectObject(oldbrush);
     br.DeleteObject();

     r.left = r.right;
     r.right = dis->rcItem.right;
     w = dc->GetTextExtent(_T(" "), 1);
     r.left += w.cx;

     CString s;
     GetLBText(dis->itemID, s);

     dc->TextOut(r.left, r.top, s);
    } /* has item */
 
 if(dis->itemState & ODS_FOCUS && dis->itemAction != ODA_SELECT)
    dc->DrawFocusRect(&r);
}

int CColor::CompareItem(LPCOMPAREITEMSTRUCT cis) 
{
        // TODO: Add your code to determine the sorting order of the specified items
        // return -1 = item 1 sorts before item 2
        // return 0 = item 1 and item 2 sort the same
        // return 1 = item 1 sorts after item 2
        
        CString s1;
        CString s2;

        GetLBText(cis->itemID1, s1);
        GetLBText(cis->itemID2, s2);

        return lstrcmp(s1, s2);
}
#endif
