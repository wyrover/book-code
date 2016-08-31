#include "stdafx.h"
#include "dlgs.h"
#include "kp.h"
#include "resource.h"

/****************************************************************************
*                              loadKerningPairs
* Inputs:
*       CDC & dc: Display context
*       CTreeCtrl * tc: Tree control into which to load info
* Result: HTREEITEM
*       Tree item we inserted, or NULL
* Effect: 
*       Inserts the kerning pair info for the font
****************************************************************************/

HTREEITEM loadKerningPairs(CDC & dc, CTreeCtrl * tc)
    {
     CString s;
     CString fmt;
     UINT lastkp = (UINT)-1;
     HTREEITEM kpi;

     int count = dc.GetKerningPairs(0, NULL);

     if(count == 0)
        return NULL;

     fmt.LoadString(IDS_KERNING_PAIRS);

     s.Format(fmt, count);
     
     HTREEITEM ti = tc->InsertItem(s);

     LPKERNINGPAIR pairs = new KERNINGPAIR[count];

     dc.GetKerningPairs(count, pairs);

     for(int i = 0; i < count; i++)
        { /* show each */
         if(lastkp != pairs[i].wFirst)
            { /* change in first */
             s.Format(_T("\"%c\" 0x%02x %d"), pairs[i].wFirst, pairs[i].wFirst,
                                pairs[i].wFirst);
             kpi = tc->InsertItem(s, ti);
             lastkp = pairs[i].wFirst;
            } /* change in first */

         s.Format(_T("\"%c\" 0x%02x %d - \"%c\" 0x%02x %d: %d"), 
                                pairs[i].wFirst, 
                                pairs[i].wFirst, 
                                pairs[i].wFirst, 
                                pairs[i].wSecond,
                                pairs[i].wSecond,
                                pairs[i].wSecond,
                                pairs[i].iKernAmount);
         tc->InsertItem(s, kpi);
        } /* show each */

     delete pairs;

     return ti;
    }
