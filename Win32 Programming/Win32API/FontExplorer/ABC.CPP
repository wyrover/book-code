#include "stdafx.h"
#include "chformat.h"

/****************************************************************************
*                                loadABCWidths
* Inputs:
*       CDC * dc: DC to use for getting widths
*       CString smp: Sample string
*       CTreeCtrl * tc: Tree control into which to insert it
*       HTREEITEM wd: Parent node of width
* Result: void
*       
* Effect: 
*       Computes the ABC widths, in floating point if possible, and displays
*       the integer or integer and floating point versions of the widths.
****************************************************************************/

void loadABCWidths(CDC * dc, CString smp, CTreeCtrl * tc, HTREEITEM wd)
    {     
     HTREEITEM gabc = tc->InsertItem(_T("GetCharABCWidths"), wd);
     CString s;

     for(int i = 0; i < smp.GetLength(); i++)
        { /* add ABC */
         ABC abc;
         ABCFLOAT abcf;
         BOOL flt;

         dc->GetCharABCWidths(smp[i], smp[i], &abc);
         flt = ::GetCharABCWidthsFloat(dc->m_hDC, smp[i], smp[i], &abcf);

         charFormat(s, smp[i]);


         HTREEITEM iabc = tc->InsertItem(s, gabc);
         if(flt)
            { /* floating */
#define FloatFormat _T("%c %d   %#0.4g")
             s.Format(FloatFormat, _T('A'), abc.abcA, abcf.abcfA);
             tc->InsertItem(s, iabc);

             s.Format(FloatFormat, _T('B'), abc.abcB, abcf.abcfB);
             tc->InsertItem(s, iabc);

             s.Format(FloatFormat, _T('C'), abc.abcC, abcf.abcfC);
             tc->InsertItem(s, iabc);
            } /* floating */
         else
            { /* fixed only */
             s.Format(_T("A %d"), abc.abcA);
             tc->InsertItem(s, iabc);

             s.Format(_T("B %d"), abc.abcB);
             tc->InsertItem(s, iabc);

             s.Format(_T("C %d"), abc.abcC);
             tc->InsertItem(s, iabc);
            } /* fixed only */
        } /* add ABC */
     
    }
