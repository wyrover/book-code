// Color.cpp : implementation file
//

#include "stdafx.h"
#include "CtlColorExplorer.h"
#include "Color.h"
#include "colors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct { UINT str; COLORREF color; } ColorTable;

static ColorTable colors[] = {
        { IDS_NONE,    COLOR_NONE        },
        { IDS_BLACK,   RGB(  0,   0,   0)},
        { IDS_MAROON,  RGB(128,   0,   0)},
        { IDS_GREEN,   RGB(  0, 128,   0)},
        { IDS_OLIVE,   RGB(128, 128,   0)},
        { IDS_NAVY,    RGB(  0,   0, 128)},
        { IDS_PURPLE,  RGB(128,   0, 128)},
        { IDS_TEAL,    RGB(  0, 128, 128)},
        { IDS_GRAY,    RGB(128, 128, 128)},
        { IDS_LTGRAY,  RGB(192, 192, 192)},  // aka Silver
        { IDS_RED,     RGB(255,   0,   0)},
        { IDS_LIME,    RGB(0,   255,   0)},
        { IDS_YELLOW,  RGB(255, 255,   0)},
        { IDS_BLUE,    RGB(  0,   0, 255)},
        { IDS_MAGENTA, RGB(255,   0, 255)}, // aka Fuschia
        { IDS_CYAN,    RGB(  0, 255, 255)}, // aka Aqua
        { IDS_WHITE,   RGB(255, 255, 255)},
        { IDS_DKGRAY,  RGB( 63,  63,  63)},
        { 0,           RGB(  0,   0,   0)} // EOT - value not used
                      };


/////////////////////////////////////////////////////////////////////////////
// CColor

CColor::CColor()
{
}

CColor::~CColor()
{
}


BEGIN_MESSAGE_MAP(CColor, CComboBox)
        //{{AFX_MSG_MAP(CColor)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColor message handlers

/****************************************************************************
*                             CColor::LoadColors
* Result: void
*       
* Effect: 
*       Loads the control with the colors from the color table
****************************************************************************/

void CColor::LoadColors()
    {
     int i;
     CString s;

     for(i = 0; colors[i].str != 0; i++)
        { /* load combo */
         s.LoadString(colors[i].str);
         int index = AddString(s);
         SetItemData(index, colors[i].color);
        } /* load combo */
    }

/****************************************************************************
*                              CColor::SetColor
* Inputs:
*       COLORREF color
* Result: int
*       Index of item selected, or CB_ERR if not found
* Effect: 
*       Selects the color specified from the ComboBox.  If CB_ERR is returned,
*       the current value is not changed.
****************************************************************************/

int CColor::SetColor(COLORREF color)
    {
     int i;
     for(i = 0; i < GetCount(); i++)
        { /* scan */
         if(GetItemData(i) == color)
            { /* select it */
             SetCurSel(i);
             return i;
            } /* select it */
        } /* scan */
     return CB_ERR;
    }

/****************************************************************************
*                              CColor::GetColor
* Result: COLORREF
*       The selected color, or COLOR_NONE if no selection active or selection
*       is "None"
****************************************************************************/

COLORREF CColor::GetColor()
    {
     int i = GetCurSel();
     if(i == CB_ERR)
        return COLOR_NONE;
     COLORREF color = (COLORREF)GetItemData(i);
     return color;
    }

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
