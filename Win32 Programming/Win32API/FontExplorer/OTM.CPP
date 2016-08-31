#include "stdafx.h"
#include "otm.h"
#include "resource.h"
#include "panose.h"

/****************************************************************************
*                               fsTypeToEntries
* Inputs:
*       UINT fstype: fs type code
*       HTREEITEM ti: Tree item below which to insert the codes
* Result: void
*       
* Effect: 
*       Creates an explanation as a subtree of the specified node
****************************************************************************/

static void fsTypeToEntries(UINT fstype, CTreeCtrl * tc, HTREEITEM ti)
    {
     CString s;
     CString t;

     // bits  hex & 0x6
     // ----  ----------
     // 000x   00       EMBEDDABLE
     // 001x   02       NOT EMBEDDABLE
     // 010x   04       READ-ONLY EMBEDDABLE
     // 100x   08       <<unknown>> undocumented bit found in some fonts

     if(!(fstype & 0x02))
        { /* embeddable */
         t.LoadString(IDS_FSTYPE_EMBEDDABLE);
         s.Format(_T("%02x %s"), 0, t);
         tc->InsertItem(s, ti);
        } /* embeddable */
     else
        { /* not embeddable */
         t.LoadString(IDS_FSTYPE_NOT_EMBEDDABLE);
         s.Format(_T("%02x %s"), 0x02, t);
         tc->InsertItem(s, ti);
         fstype &= ~0x02;
        } /* not embeddable */

     if(fstype & 0x04)
        { /* ro embeddable */
         t.LoadString(IDS_FSTYPE_RO_EMBEDDABLE);
         s.Format(_T("%02x %s"), 0x04, t);
         fstype &= 0x04;
         tc->InsertItem(s, ti);
        } /* ro embeddable */

     if(fstype != 0)
        { /* unknown bits */
         t.Format(_T("%02x"), fstype);
         tc->InsertItem(t, ti);
        } /* unknown bits */
    }

/****************************************************************************
*                                    fsint
* Inputs:
*       UINT & fss: 
*       UINT id:
*       UINT mask: Bit mask
* Result: CString
*       
* Effect:
*       If the bit specified by mask appears in the fss, the string whose
*       id is specified is returned, and the bit is removed from the fss.
****************************************************************************/

static CString fsint(UINT& fss, UINT id, UINT mask)
    {
     CString s;
     s = _T("");
     if(fss & mask)
        { /* load it */
         CString t;
         t.LoadString(id);
         s.Format("%02x %s", mask, (LPCTSTR)t);
        } /* load it */
     fss &= ~mask;
     return s;
    }

/****************************************************************************
*                                 otmToString
* Inputs:
*       LPOUTLINETEXTMETRIC otm: pointer to OTM
*       PSTR offset: Offset value to string 
* Result: LPCTSTR
*       Pointer to the string addressed
****************************************************************************/

static LPCTSTR otmToString(LPOUTLINETEXTMETRIC otm, PSTR offset)
    {
     LPSTR p = (LPSTR)otm;
     return (LPCTSTR)&p[(UINT)offset];
    }

/****************************************************************************
*                    CFontExplorer::loadOutlineTextMetrics
* Inputs:
*       CTreeCtrl * tc: Tree control
* Result: HTREEITEM
*       Tree item where it was inserted, or NULL
* Effect: 
*       Loads the ABCWidths data
****************************************************************************/

HTREEITEM loadOutlineTextMetrics(LPOUTLINETEXTMETRIC otm, CTreeCtrl * tc)
    {
     CString s;
     HTREEITEM ti;

     if(otm == NULL)
        return NULL;  // wasn't a TrueType font
        
     ti = tc->InsertItem(_T("OUTLINETEXTMETRIC"));
     
     s.Format(_T("otmSize %d"), otm->otmSize);
     tc->InsertItem(s, ti);

     s.Format(_T("otmTextMetrics"));
     tc->InsertItem(s, ti);

     s.Format(_T("otmFiller"));
     tc->InsertItem(s, ti);

     s.Format(_T("otmPanoseNumber"));
     HTREEITEM pn = tc->InsertItem(s, ti);
     
     loadPanose(tc, pn, &otm->otmPanoseNumber);


     s.Format(_T("otmfsSelection 0x%02x"), otm->otmfsSelection);
     HTREEITEM fs = tc->InsertItem(s, ti);
         {
          UINT fss = otm->otmfsSelection;

          s = fsint(fss, IDS_FSSELECTION_ITALIC,     0x01);
          if(s.GetLength() > 0)
             tc->InsertItem(s, fs);

          s = fsint(fss, IDS_FSSELECTION_UNDERSCORE, 0x02);
          if(s.GetLength() > 0)
             tc->InsertItem(s, fs);

          s = fsint(fss, IDS_FSSELECTION_NEGATIVE,   0x04);
          if(s.GetLength() > 0)
             tc->InsertItem(s, fs);

          s = fsint(fss, IDS_FSSELECTION_OUTLINE,    0x08);
          if(s.GetLength() > 0)
             tc->InsertItem(s, fs);

          s = fsint(fss, IDS_FSSELECTION_STRIKEOUT,  0x10);
          if(s.GetLength() > 0)
             tc->InsertItem(s, fs);

          s = fsint(fss, IDS_FSSELECTION_BOLD,       0x20);
          if(s.GetLength() > 0)
             tc->InsertItem(s, fs);

          if(fss != 0)
             { /* left over */
              s.FormatMessage(IDS_FSSELECTION_UNKNOWN, fss);
              tc->InsertItem(s, fs);
             } /* left over */
         }


     s.Format(_T("otmfsType 0x%02x"), otm->otmfsType);
     HTREEITEM fst = tc->InsertItem(s, ti);

        fsTypeToEntries(otm->otmfsType, tc, fst);

     s.Format(_T("otmsCharSlopeRise %d"), otm->otmsCharSlopeRise);
     tc->InsertItem(s, ti);

     s.Format(_T("otmsCharSlopeRun %d"), otm->otmsCharSlopeRun);
     tc->InsertItem(s, ti);

     s.Format(_T("otmItalicAngle %d"), otm->otmItalicAngle);
     tc->InsertItem(s, ti);

     s.Format(_T("otmEMSquare %u"), otm->otmEMSquare);
     tc->InsertItem(s, ti);

     s.Format(_T("otmAscent %d"), otm->otmAscent);
     tc->InsertItem(s, ti);

     s.Format(_T("otmLineGap %u"), otm->otmLineGap);
     tc->InsertItem(s, ti);

     s.Format(_T("otmsCapEmHeight %d"), otm->otmsCapEmHeight);
     tc->InsertItem(s, ti);

     s.Format(_T("otmsXHeight %u"), otm->otmsXHeight);
     tc->InsertItem(s, ti);

     HTREEITEM fb = tc->InsertItem(_T("otmrcFontBox"), ti);
         s.Format(_T("left %d"),otm->otmrcFontBox.left);
         tc->InsertItem(s, fb);
         s.Format(_T("top %d"),otm->otmrcFontBox.top);
         tc->InsertItem(s, fb);
         s.Format(_T("right %d"),otm->otmrcFontBox.right);
         tc->InsertItem(s, fb);
         s.Format(_T("bottom %d"),otm->otmrcFontBox.bottom);
         tc->InsertItem(s, fb);

     s.Format(_T("otmMacAscent %d"), otm->otmMacAscent);
     tc->InsertItem(s, ti);

     s.Format(_T("otmMacDescent %d"), otm->otmMacDescent);
     tc->InsertItem(s, ti);

     s.Format(_T("otmMacLineGap %u"), otm->otmMacLineGap);
     tc->InsertItem(s, ti);

     s.Format(_T("otmusMinimumPPEM %d"), otm->otmusMinimumPPEM);
     tc->InsertItem(s, ti);

     fb = tc->InsertItem(_T("otmptSubscriptSize"), ti);
         s.Format(_T("x %d"),otm->otmptSubscriptSize.x);
         tc->InsertItem(s, fb);
         s.Format(_T("y %d"),otm->otmptSubscriptSize.y);
         tc->InsertItem(s, fb);

     fb = tc->InsertItem(_T("otmptSubscriptOffset"), ti); 
         s.Format(_T("x %d"),otm->otmptSubscriptOffset.x);
         tc->InsertItem(s, fb);
         s.Format(_T("y %d"),otm->otmptSubscriptOffset.y);
         tc->InsertItem(s, fb);

     fb = tc->InsertItem(_T("otmptSuperscriptSize"), ti);
         s.Format(_T("x %d"),otm->otmptSuperscriptSize.x);
         tc->InsertItem(s, fb);
         s.Format(_T("y %d"),otm->otmptSuperscriptSize.y);
         tc->InsertItem(s, fb);

     fb = tc->InsertItem(_T("otmptSuperscriptOffset"), ti); 
         s.Format(_T("x %d"),otm->otmptSuperscriptOffset.x);
         tc->InsertItem(s, fb);
         s.Format(_T("y %d"),otm->otmptSuperscriptOffset.y);
         tc->InsertItem(s, fb);

     s.Format(_T("otmsStrikeoutSize %u"), otm->otmsStrikeoutSize);
     tc->InsertItem(s, ti);

     s.Format(_T("otmsStrikeoutPosition %d"), otm->otmsStrikeoutPosition);
     tc->InsertItem(s, ti);

     s.Format(_T("otmsUnderscoreSize %u"), otm->otmsUnderscoreSize);
     tc->InsertItem(s, ti);

     s.Format(_T("otmsUnderscorePosition %d"), otm->otmsUnderscorePosition);
     tc->InsertItem(s, ti);

     s.Format(_T("otmpFamilyName \"%s\""), otmToString(otm, otm->otmpFamilyName));
     tc->InsertItem(s, ti);

     s.Format(_T("otmpFaceName \"%s\""), otmToString(otm, otm->otmpFaceName));
     tc->InsertItem(s, ti);

     s.Format(_T("otmpStyleName \"%s\""), otmToString(otm, otm->otmpStyleName));
     tc->InsertItem(s, ti);

     s.Format(_T("otmpFullName \"%s\""), otmToString(otm, otm->otmpFullName));
     tc->InsertItem(s, ti);

     return ti;
    }
