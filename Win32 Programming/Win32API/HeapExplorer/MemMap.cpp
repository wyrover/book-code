// MemMap.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "mystring.h"
#include "legendi.h"
#include "data.h"
#include "MemMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static LegendInfo * legend[] = {
        &Legend_1, 
        &Legend_1_Ovr, 
        &Legend_2, 
        &Legend_2_Ovr, 
        &Legend_Free, 
        &Legend_Overhead, 
        &Legend_Filler, 
        NULL // EOT
                               };

/****************************************************************************
*                            CMemoryMap::getLegend
* Result: LegendInfo *
*       
****************************************************************************/

LegendInfo ** CMemoryMap::getLegend()
    {
     return &legend[0];
    }


/////////////////////////////////////////////////////////////////////////////
// CMemoryMap

CMemoryMap::CMemoryMap()
{
}

CMemoryMap::~CMemoryMap()
{
}


BEGIN_MESSAGE_MAP(CMemoryMap, CStatic)
        //{{AFX_MSG_MAP(CMemoryMap)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryMap message handlers

void CMemoryMap::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        
        CRect r;
        GetClientRect(&r);

        HANDLE heap = CMyData::getHeapHandle();

        PROCESS_HEAP_ENTRY e;
        int space;

        CBrush br(::GetSysColor(COLOR_BTNFACE));
        dc.SelectStockObject(NULL_PEN);
        dc.SelectObject(&br);
        dc.Rectangle(&r);

        if(heap == NULL)
           { /* no data */
            return;
           } /* no data */

        r.InflateRect(-2, -2);

        e.lpData = NULL; // initialize loop
        space = 0;

        while(HeapWalk(heap, &e))
           { /* count space */
            if(e.wFlags == 0)
               { /* free */
                space += e.cbOverhead + e.cbData;
               } /* free */
            else
            if(e.wFlags & PROCESS_HEAP_REGION)
               { /* region header */
                space += e.cbOverhead + e.cbData;
               } /* region header */
            else
            if(e.wFlags & PROCESS_HEAP_ENTRY_BUSY)
               { /* occupied */
                space += e.cbOverhead + e.cbData;
               } /* occupied */
           } /* count space */

        // Now compute how many stripes we need to represent this
        int stripes = (space / r.Width()) + 1;
        int height = r.Height() / stripes;

        // Now draw it
        int hpos = 0;
        int vpos = 0;
        int color = 0;
        LegendInfo * li = NULL;
        LegendInfo * lo = NULL;
        BOOL needFiller = FALSE;
        DWORD len;
        DWORD end;

        e.lpData = NULL; // initialize loop
        while(HeapWalk(heap, &e))
           { /* draw space */
            CRect d;
            int alloc;
            int ovr;
            color++;

            if(!needFiller == 0)
               end = (DWORD)e.lpData;
            needFiller = TRUE;

            if(e.wFlags == 0)
               { /* free space */
                alloc = e.cbData;
                ovr = e.cbOverhead;
                li = &Legend_Free;
                lo = &Legend_Overhead;
               } /* free space */
            else
            if(e.wFlags & PROCESS_HEAP_REGION)
               { /* region header */
                alloc = e.cbData;
                ovr = e.cbOverhead;
                li = &Legend_Admin;
                lo = &Legend_Overhead;
                needFiller = FALSE;
               } /* region header */
            else
            if(e.wFlags & PROCESS_HEAP_ENTRY_BUSY)
               { /* allocated */
                alloc = e.cbData;
                ovr = e.cbOverhead;
                li = (color & 1) ? &Legend_1 : &Legend_2;
                lo = (color & 1) ? &Legend_1_Ovr : &Legend_2_Ovr;
               } /* allocated */
            else
            if(e.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE)
               { /* uncommitted */
                alloc = e.cbData;
                ovr = e.cbOverhead;
                li = NULL;
                lo = NULL;
               } /* uncommitted */

#define drawRect(var, legend) \
                if(legend != NULL) \
                   { /* select it */ \
                    dc.SelectObject((legend)->pen); \
                    dc.SelectObject((legend)->brush); \
                   } /* select it */ \
                if(var > 0) \
                   { /* draw var */ \
                    d.left = hpos; \
                    d.top = vpos; \
                    d.right = d.left + var; \
                    d.bottom = d.top + height; \
                    if(d.right > r.right) \
                       { /* residual block */ \
                        int delta = (r.right - d.right); \
                        d.right -= delta; \
                        dc.Rectangle(&d); \
                        hpos = 0; \
                        vpos += height; \
                        var = delta; \
                        continue; \
                       } /* residual block */ \
                    else \
                       { /* block fits */ \
                        dc.Rectangle(&d); \
                        hpos = d.right; \
                        var = 0; \
                       } /* block fits */ \
                   } /* draw var */


            len = ovr + alloc;

            int filler = (int)((DWORD)e.lpData - end);

            while(needFiller && filler > 0)
               { /* draw filler */
                drawRect(filler, &Legend_Filler);
               } /* draw filler */

            while(ovr + alloc > 0)
               { /* draw stripes */
                drawRect(alloc, li);
                drawRect(ovr, lo);
               } /* draw stripes */
            
            end = (DWORD)e.lpData + len;
           } /* draw space */
}
