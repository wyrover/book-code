#include "stdafx.h"
#include "dlgs.h"
#include "logfont.h"
#include "tm.h"
#include "resource.h"

/****************************************************************************
*                                 addFlagVal
* Inputs:
*       CTreeCtrl * tc: Tree control
*       HTREEITEM flags: Place to add it in tree
*       DWORD val: Flag value
*       DWORD mask: mask
*       UINT id: String resource id
* Result: void
*       
* Effect: 
*       Adds the item if the flag bit is set
****************************************************************************/

void addFlagVal(CTreeCtrl * tc, HTREEITEM flags, DWORD val, DWORD mask, UINT id)
    {
     CString s;
     CString fmt;

     if((val & mask) == 0)
        return;   // bit not present

     fmt.LoadString(id);
     s.Format(fmt, mask);
     tc->InsertItem(s, flags);
    }

/****************************************************************************
*                               loadTextMetrics
* Inputs:
*       TEXTMETRIC * tm: Text metric data
*       CTreeCtrl * tc: Tree control to add items to
* Result: HTREEITEM
*       Handle on TEXTMETRIC item
* Effect: 
*       Loads the textmetrics data
****************************************************************************/

HTREEITEM loadTextMetrics(TEXTMETRIC * tm, CTreeCtrl * tc )
    {
     HTREEITEM ti;
     CString s;
     if(tm->tmPitchAndFamily & TMPF_TRUETYPE)
        ti = tc->InsertItem(_T("NEWTEXTMETRIC"));
     else
        ti = tc->InsertItem(_T("TEXTMETRIC"));

     s.Format(_T("tmHeight: %d"), tm->tmHeight);
     tc->InsertItem(s, ti);

     s.Format(_T("tmAscent: %d"), tm->tmAscent);
     tc->InsertItem(s, ti);

     s.Format(_T("tmDescent: %d"), tm->tmDescent);
     tc->InsertItem(s, ti);
      
     s.Format(_T("tmInternalLeading: %d"), tm->tmInternalLeading);
     tc->InsertItem(s, ti);

     s.Format(_T("tmExternalLeading: %d"), tm->tmExternalLeading);
     tc->InsertItem(s, ti);

     s.Format(_T("tmAveCharWidth: %d"), tm->tmAveCharWidth);
     tc->InsertItem(s, ti);

     s.Format(_T("tmMaxCharWidth: %d"), tm->tmMaxCharWidth);
     tc->InsertItem(s, ti);

     s.Format(_T("tmWeight: %d"), tm->tmWeight);
     HTREEITEM wt = tc->InsertItem(s, ti);

     s = weightToString(tm->tmWeight);
     if(s.GetLength() != 0)
        tc->InsertItem(s, wt);

     s.Format(_T("tmOverhang: %d"), tm->tmOverhang);
     tc->InsertItem(s, ti);

     s.Format(_T("tmDigitizedAspectX: %d"), tm->tmDigitizedAspectX);
     tc->InsertItem(s, ti);

     s.Format(_T("tmDigitizedAspectY: %d"), tm->tmDigitizedAspectY);
     tc->InsertItem(s, ti);

     s.Format(_T("tmFirstChar: %d"), tm->tmFirstChar);
     tc->InsertItem(s, ti);

     s.Format(_T("tmLastChar: %d"), tm->tmLastChar);
     tc->InsertItem(s, ti);

     s.Format(_T("tmDefaultChar: %d"), tm->tmDefaultChar);
     tc->InsertItem(s, ti);

     s.Format(_T("tmBreakChar: %d"), tm->tmBreakChar);
     tc->InsertItem(s, ti);

     s.Format(_T("tmItalic: %d"), tm->tmItalic);
     tc->InsertItem(s, ti);

     s.Format(_T("tmUnderlined: %d"), tm->tmUnderlined);
     tc->InsertItem(s, ti);

     s.Format(_T("tmStruckOut: %d"), tm->tmStruckOut);
     tc->InsertItem(s, ti);

     s.Format(_T("tmPitchAndFamily: 0x%02x"), tm->tmPitchAndFamily);
     HTREEITEM paf = tc->InsertItem(s, ti);
     insertPitchAndFamily(tm->tmPitchAndFamily, tc, paf);

     s.Format(_T("tmCharSet: %d"), tm->tmCharSet);
     tc->InsertItem(s, ti);

     if(tm->tmPitchAndFamily & TMPF_TRUETYPE)
        { /* newtextmetric */
         LPNEWTEXTMETRIC ntm = (LPNEWTEXTMETRIC)tm;
      
         s.Format(_T("ntmFlags 0x%02x"), ntm->ntmFlags);
         HTREEITEM flags = tc->InsertItem(s, ti);

         addFlagVal(tc, flags, ntm->ntmFlags, 0x01, IDS_NTM_ITALIC);
         addFlagVal(tc, flags, ntm->ntmFlags, 0x02, IDS_NTM_UNDERSCORE);
         addFlagVal(tc, flags, ntm->ntmFlags, 0x04, IDS_NTM_NEGATIVE);
         addFlagVal(tc, flags, ntm->ntmFlags, 0x08, IDS_NTM_OUTLINE);
         addFlagVal(tc, flags, ntm->ntmFlags, 0x10, IDS_NTM_STRIKEOUT);
         addFlagVal(tc, flags, ntm->ntmFlags, 0x20, IDS_NTM_BOLD);
          
         s.Format(_T("ntmSizeEM %d"), ntm->ntmSizeEM);
         tc->InsertItem(s, ti);

         s.Format(_T("ntmCellHeight %d"), ntm->ntmCellHeight);
         tc->InsertItem(s, ti);

         s.Format(_T("ntmAvgWIdth %d"), ntm->ntmAvgWidth);
         tc->InsertItem(s, ti);
        } /* newtextmetric */
     
     return ti;
    }
