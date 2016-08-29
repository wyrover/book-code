#include "stdafx.h"
#include "fontdesc.h"
#include "color.h"

/****************************************************************************
*                              CFontDescriptor::CFontDescriptor
* Inputs:
*       CWnd * wnd: Window handle for window
* Effect: 
*       Creates a CFontDescriptor.  
****************************************************************************/

CFontDescriptor::CFontDescriptor(CWnd * wnd, CWnd * fontwnd /* = NULL */)
    {
     memset(&abc, 0, sizeof(abc)); // font-sample-specific
     memset(&tm, 0, sizeof(tm));
     memset(&otm, 0, sizeof(otm));
     otm = NULL;

     if(fontwnd != NULL)
        { /* has parent */
         // If it has a parent, the parent must process the UWM_GETCOLOR
         // message (default will return 0, that is RGB(0,0,0)
         color = fontwnd->SendMessage(UWM_GETCOLOR);
        } /* has parent */
     else
        { /* no color */
         color = ::GetSysColor(COLOR_WINDOWTEXT);
        } /* no color */

     dc = new CClientDC(wnd);

     memset(&lf, 0, sizeof(LOGFONT));

     (fontwnd == NULL ? wnd : fontwnd)->SendMessage(WM_CHOOSEFONT_GETLOGFONT, 0, (LPARAM)&lf);
     saved = dc->SaveDC();
     font = new CFont();

     if(font->CreateFontIndirect(&lf))
        { /* successful creation */
         dc->SelectObject(font);
         dc->GetTextMetrics(&tm);
         int n = dc->GetOutlineTextMetrics(0, NULL);
         if(n > 0)
            { /* load it */
             otm = (OUTLINETEXTMETRIC *)new BYTE[n];
             dc->GetOutlineTextMetrics(n, otm);
            } /* load it */
         return;
        } /* successful creation */

     // If unsuccessful in creating the font, restore the DC and delete it
     dc->RestoreDC(saved);
     delete dc;
     dc = NULL;
     saved = -1;
    }

/****************************************************************************
*                              CFontDescriptor::~CFontDescriptor
* Effect: 
*       Destructor.  Releases the DC, deletes the font, if any
****************************************************************************/

CFontDescriptor::~CFontDescriptor()
    {
     if(saved >= 0)
        { /* restore DC */
         dc->RestoreDC(saved);
        } /* restore DC */

     delete font;
     delete dc;
     delete [] (LPBYTE)otm;
    }
