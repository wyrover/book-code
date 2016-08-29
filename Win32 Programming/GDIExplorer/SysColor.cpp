// SysColor.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "SysColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysColor

CSysColor::CSysColor()
{
 maxlen = -1;
}

CSysColor::~CSysColor()
{
}

static IDData colors[] = {
        { IDS_COLOR_SCROLLBAR,          COLOR_SCROLLBAR},
        { IDS_COLOR_BACKGROUND,         COLOR_BACKGROUND},
        { IDS_COLOR_ACTIVECAPTION,      COLOR_ACTIVECAPTION},
        { IDS_COLOR_INACTIVECAPTION,    COLOR_INACTIVECAPTION},
        { IDS_COLOR_MENU,               COLOR_MENU},
        { IDS_COLOR_WINDOW,             COLOR_WINDOW},
        { IDS_COLOR_WINDOWFRAME,        COLOR_WINDOWFRAME},
        { IDS_COLOR_MENUTEXT,           COLOR_MENUTEXT},
        { IDS_COLOR_WINDOWTEXT,         COLOR_WINDOWTEXT},
        { IDS_COLOR_CAPTIONTEXT,        COLOR_CAPTIONTEXT},
        { IDS_COLOR_ACTIVEBORDER,       COLOR_ACTIVEBORDER},
        { IDS_COLOR_INACTIVEBORDER,     COLOR_INACTIVEBORDER},
        { IDS_COLOR_APPWORKSPACE,       COLOR_APPWORKSPACE},
        { IDS_COLOR_HIGHLIGHT,          COLOR_HIGHLIGHT},
        { IDS_COLOR_HIGHLIGHTTEXT,      COLOR_HIGHLIGHTTEXT},
        { IDS_COLOR_BTNFACE,            COLOR_BTNFACE},
        { IDS_COLOR_BTNSHADOW,          COLOR_BTNSHADOW},
        { IDS_COLOR_GRAYTEXT,           COLOR_GRAYTEXT},
        { IDS_COLOR_BTNTEXT,            COLOR_BTNTEXT},
        { IDS_COLOR_INACTIVECAPTIONTEXT,COLOR_INACTIVECAPTIONTEXT},
        { IDS_COLOR_BTNHIGHLIGHT,       COLOR_BTNHIGHLIGHT},
// These colors apply only to Win 4.x and later
        { IDS_COLOR_3DDKSHADOW,         COLOR_3DDKSHADOW},
        { IDS_COLOR_3DLIGHT,            COLOR_3DLIGHT},
        { IDS_COLOR_INFOTEXT,           COLOR_INFOTEXT},
        { IDS_COLOR_INFOBK,             COLOR_INFOBK},
// These colors are synonyms
        { IDS_COLOR_DESKTOP,            COLOR_DESKTOP},
        { IDS_COLOR_3DFACE,             COLOR_3DFACE},
        { IDS_COLOR_3DSHADOW,           COLOR_3DSHADOW},
        { IDS_COLOR_3DHIGHLIGHT,        COLOR_3DHIGHLIGHT},
        { 0, 0} // EOT
                         };

BEGIN_MESSAGE_MAP(CSysColor, CColor)
        //{{AFX_MSG_MAP(CSysColor)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysColor message handlers

/****************************************************************************
*                               CSysColor::Load
* Inputs:
*       int sel: Desired selection ID.  Must be in stock color list
* Result: int
*       Actual selection made
* Effect: 
*       Loads all the colors, including the system colors
****************************************************************************/

int CSysColor::Load(int sel)
    {
     int found = CColor::Load(sel);
     CString s;

     if(found != CB_ERR)
        GetLBText(found, s);  // save caption for later match
     else
        { /* default stock */
         GetLBText(0, s);      // default is first color in stock list
         found = 0;
        } /* default stock */

     // The table we have uses color indices.  We have to change them
     // to colors to add them.  We make a copy of the entry, change its
     // color value, and add the entry.

     for(int i = 0; colors[i].id != 0; i++)
        { /* add each */
         IDData d = colors[i];
         d.val = ::GetSysColor(d.val);
         // test d.val here for validity, if valid
         CColor::AddString(&d);
        } /* add each */
         

     // If the color is found in the stock colors, we don't need to
     // load it.  Note that we should ideally never hit the first
     // condition below because we force the selection of the first element

     if(found == CB_ERR)
        { /* default first */
         SetCurSel(0);  // default to first item in list
         return 0;
        } /* default first */
     else
        { /* reselect */
         sel = FindStringExact(-1, s);
         SetCurSel(sel);
         return sel;
        } /* reselect */

    }
