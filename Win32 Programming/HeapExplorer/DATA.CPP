// data.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "legendi.h"
#include "data.h"
#include "infomsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


alloc::alloc()
    {
     seq = 0;
     count = 0;
     lasterr = 0;
     here = NULL;
     base = NULL;
     quantum = 1;
     memset(&ph, 0, sizeof(ph));
    }

alloc::~alloc()
    {
    }

/* Color scheme:
   Nominal color        Code            Meaning

   Red seq #            dataCOLOR_ERR   Allocation error
   White seq #          dataCOLOR_OK    Allocation success

   Gray                 dataCOLOR_FREE  Free block
   Yellow               dataCOLOR_1     Allocated color 1
   Green                dataCOLOR_2     Allocated color 2
   Blue                 dataCOLOR_THIS  Color of most recent allocation

   Black                dataCOLOR_HDR
   Red                  data_COLOR_PAGE
*/
#define dataCOLOR_ERR       RGB(255,   0,   0)
#define dataCOLOR_OK        RGB(255, 255, 255)
#define dataCOLOR_FREE      RGB(192, 192, 192)
#define dataCOLOR_1         RGB(255, 255,   0)
#define dataCOLOR_2         RGB(  0, 255,   0)
#define dataCOLOR_TEXT1     RGB(  0,   0,   0)
#define dataCOLOR_TEXT2     RGB(  0,   0,   0)
#define dataCOLOR_TEXT_THIS RGB(255, 255, 255)
#define dataCOLOR_THIS      RGB(  0,   0, 255)
#define dataCOLOR_TEXTOK    RGB(  0,   0,   0)
#define dataCOLOR_TEXTERR   RGB(255, 255,   0)
#define dataCOLOR_HDR       RGB(  0,   0,   0)
#define dataCOLOR_ADMIN     RGB(  0,   0,   0)
#define dataCOLOR_PAGE      RGB(255,   0,   0)
#define dataCOLOR_FILLER    RGB(255,   0, 255)
#define dataCOLOR_OVR1      RGB(192, 192,   0)
#define dataCOLOR_OVR2      RGB(  0, 128,   0)

static CPen pagePen(PS_SOLID, 0, dataCOLOR_PAGE);
static CPen adminPen(PS_SOLID, 0, dataCOLOR_FREE);
static CBrush adminBrush(HS_BDIAGONAL, dataCOLOR_ADMIN);
static CPen hdrPen(PS_SOLID, 0, dataCOLOR_HDR);
static CBrush hdrBrush(dataCOLOR_HDR);
static CPen thisPen(PS_SOLID, 0, dataCOLOR_THIS);
static CBrush thisBrush(dataCOLOR_THIS);

static CBrush block1Brush(dataCOLOR_1);
static CBrush block2Brush(dataCOLOR_2);
static CPen   block1Pen(PS_SOLID, 0, dataCOLOR_1);
static CPen   block2Pen(PS_SOLID, 0, dataCOLOR_2);
static CBrush fillerBrush(dataCOLOR_FILLER);
static CPen   fillerPen(PS_SOLID, 0, dataCOLOR_FILLER);

static CBrush ovr1Brush(dataCOLOR_OVR1);
static CBrush ovr2Brush(dataCOLOR_OVR2);
static CPen   ovr1Pen(PS_SOLID, 0, dataCOLOR_OVR1);
static CPen   ovr2Pen(PS_SOLID, 0, dataCOLOR_OVR2);
static struct {
                CBrush * brush;
                CPen   * pen;
                COLORREF textcolor;
              } blockObjects[2] = {
                                {&block1Brush, &block1Pen, dataCOLOR_TEXT1},
                                {&block2Brush, &block2Pen, dataCOLOR_TEXT2}
                                  };
static CBrush freeBrush(dataCOLOR_FREE);
static CPen   freePen(PS_SOLID, 0, dataCOLOR_FREE);

LegendInfo Legend_1(&block1Brush, &block1Pen, IDS_LEGEND_1);
LegendInfo Legend_2(&block2Brush, &block2Pen, IDS_LEGEND_2);
LegendInfo Legend_1_Ovr(&ovr1Brush, &ovr1Pen, IDS_LEGEND_1_OVER);
LegendInfo Legend_2_Ovr(&ovr2Brush, &ovr2Pen, IDS_LEGEND_2_OVER);
LegendInfo Legend_Free(&freeBrush,   &freePen,   IDS_LEGEND_FREE);
LegendInfo Legend_This(&thisBrush,   &thisPen,   IDS_LEGEND_THIS);
LegendInfo Legend_Admin(&adminBrush,  &adminPen,  IDS_LEGEND_ADMIN);
LegendInfo Legend_Overhead(&hdrBrush,    &hdrPen,    IDS_LEGEND_OVERHEAD);
LegendInfo Legend_Filler(&fillerBrush, &fillerPen, IDS_LEGEND_FILLER);

static LegendInfo * legend[] = {
        &Legend_1, &Legend_2, &Legend_Free, &Legend_This, &Legend_Admin, 
                                &Legend_Overhead, NULL};


/****************************************************************************
*                            CDataList::GetLegend
* Result: LegendInfo *
*       Pointer to legend info table
****************************************************************************/

LegendInfo ** CDataList::GetLegend()
    {
     return &legend[0];
    }


/////////////////////////////////////////////////////////////////////////////
// CDataList

CDataList::CDataList()
{
}

CDataList::~CDataList()
{
}


BEGIN_MESSAGE_MAP(CDataList, CListBox)
        //{{AFX_MSG_MAP(CDataList)
        ON_WM_RBUTTONDOWN()
        ON_WM_RBUTTONUP()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataList message handlers

/****************************************************************************
*                         CDataList::addr_to_distance
* Inputs:
*       �
* Result: �
*       �
* Effect: 
*       �
****************************************************************************/

int CDataList::addr_to_distance(alloc * data, DWORD start, DWORD end, DWORD range)
    {
     return (int)(((((float)(end - start) / (float)data->quantum)) / (float)data->range) * range) / data->pages;
    }


void CDataList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 alloc * data = (alloc *)lpDrawItemStruct->itemData;
 SIZE sz;

 if(data == NULL)
    return;

 CDC * dc = CDC::FromHandle(lpDrawItemStruct->hDC);
 RECT r;
 CBrush bk;
 CPen bar;
 CPen bkpen;
 DWORD admin = 0;
 DWORD adminovh = 0;
 DWORD adminbase = 0;
 /*
    +------------------------------------------------------------------+
    | seq |                                                            |
    +------------------------------------------------------------------+
 */

 sz = dc->GetTextExtent(_T("000 "), 4);
 r = lpDrawItemStruct->rcItem;
 r.right = sz.cx;
 CString s;
 s.Format(_T("%d "), data->seq);

 int saved = dc->SaveDC();

 dc->SetBkMode(TRANSPARENT);
 
 dc->SelectStockObject(NULL_PEN);

 if(lpDrawItemStruct->itemState & ODS_SELECTED)
    { /* selected */
     bk.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
     bkpen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_HIGHLIGHT));
     dc->SelectObject(bk);
     dc->Rectangle(&r);
     dc->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
     bar.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
     dc->SelectObject(&bk);
     dc->SelectObject(&bkpen);
    } /* selected */
 else
    { /* no selected */
     if(data->lasterr == 0)
        { /* no error */
         bk.CreateSolidBrush(dataCOLOR_OK);
         bkpen.CreatePen(PS_SOLID, 0, dataCOLOR_OK);
         dc->SetTextColor(dataCOLOR_TEXTOK);
        } /* no error */
     else
        { /* has error */
         bk.CreateSolidBrush(dataCOLOR_ERR);
         bkpen.CreatePen(PS_SOLID, 0, dataCOLOR_ERR);
         dc->SetTextColor(dataCOLOR_TEXTERR);
        } /* has error */
     bar.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
     dc->SelectObject(&bkpen);
     dc->SelectObject(&bk);
     dc->Rectangle(&r);
    } /* no selected */
    
 dc->DrawText(s, &r, DT_RIGHT);

 
 r.left = r.right;
 r.right = lpDrawItemStruct->rcItem.right;
 r.right -= GetSystemMetrics(SM_CXVSCROLL);

 dc->SelectObject(&freeBrush);
 dc->SelectObject(&freePen);
 
 dc->Rectangle(&r);

 dc->SelectObject(&bar);
 dc->MoveTo(r.left, r.top);
 dc->LineTo(r.left, r.bottom);

 int id = 0;

 for(int i = 0; i < data->count; i++)
    { /* display memory */
     RECT m;
     m = r;
     m.left = r.left + addr_to_distance(data, 
                                        (DWORD)data->base, 
                                        (DWORD)data->ph[i].lpData, 
                                        r.right - r.left);
     m.right = m.left + addr_to_distance(data, 
                                        0,
                                        data->ph[i].cbData, 
                                        r.right - r.left);
     if(data->ph[i].wFlags == 0)
        { /* free space */
         dc->SelectObject(&freePen);
         dc->SelectObject(&freeBrush);
         id++;
        } /* free space */
     else
     if(data->ph[i].wFlags & PROCESS_HEAP_REGION)
        { /* header block */
         // We don't need to do anything here because the size is 0
         admin = (DWORD)data->ph[i].lpData;
         adminovh = (DWORD)data->ph[i].cbOverhead;
         adminbase = (DWORD)data->base;
        } /* header block */
     else
     if(data->ph[i].lpData == data->here)
        { /* current allocation */
         dc->SelectObject(&thisBrush);
         dc->SelectObject(&thisPen);
         dc->SetTextColor(dataCOLOR_TEXT_THIS);
         id++;
        } /* current allocation */
     else
        { /* old allocation */
         dc->SelectObject(blockObjects[i & 1].brush);
         dc->SelectObject(blockObjects[i & 1].pen);
         dc->SetTextColor(blockObjects[i & 1].textcolor);
         id++;
        } /* old allocation */
     dc->Rectangle(&m);

     data->pr[i] = m;

     if(data->ph[i].wFlags & PROCESS_HEAP_ENTRY_BUSY) 
        { /* nonhdr */
         CString s;
         s.Format("%d", id );
         dc->DrawText(s, &m, DT_CENTER | DT_VCENTER);
        } /* nonhdr */
                                                
     RECT o;
     o.right = m.left;
     o.top = m.top;
     o.bottom = m.bottom;
     o.left = o.right - addr_to_distance(data, 
                                        0, 
                                        data->ph[i].cbOverhead, 
                                        r.right - r.left);
     if(o.right - o.left < 2)
        o.left = o.right - 2;  // allows for (right-1) feature of Rectangle

     dc->SelectObject(hdrBrush);
     dc->SelectObject(hdrPen);
     dc->Rectangle(&o);
    } /* display memory */

 // Now draw the administrative region
 
 dc->SelectObject(&adminBrush);
 dc->SelectObject(&adminPen);

 RECT a;
 a = r;
 // a.left = r.left + addr_to_distance(data, 
 //                                 0, 
 //                                 adminovh,
 //                                 r.right - r.left);

 a.right = a.left + addr_to_distance(data,
                            adminbase,
                            admin,
                            r.right - r.left);
 dc->Rectangle(&a);
 data->pr[0] = a;

 // Now draw the page breaks
 dc->SelectObject(&pagePen);

 for(int page= 1; page <= data->pages; page++)
    { /* draw page */
     RECT p = r;
     p.left += addr_to_distance(data, 0, data->pagesize * page,
                                        r.right - r.left);
     
     dc->MoveTo(p.left, p.top);
     dc->LineTo(p.left, p.bottom);
    } /* draw page */
 dc->RestoreDC(saved);
}

/****************************************************************************
*                            CDataList::AddString
* Inputs:
*       alloc * p: Pointer to value to add
* Result: int
*       Index where it is added
* Effect: 
*       Adds the element to the list box
****************************************************************************/

int CDataList::AddString(alloc * p)
    {
     return CListBox::AddString( (LPCTSTR) p);
    }

alloc * CDataList::GetItemData(int sel)
    {
     return (alloc *)(CListBox::GetItemData(sel));
    }

void CDataList::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
 delete (alloc *)(lpDeleteItemStruct->itemData);
}

void CDataList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
 CClientDC dc(this);
 TEXTMETRIC tm;

 dc.GetTextMetrics(&tm);
 lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmInternalLeading;
        
}

/****************************************************************************
*                          CDataList::OnRButtonDown
* Inputs:
*       UINT nFlags:
*       CPoint point:
* Result: void
*       
* Effect: 
*       Simulates the left button selection, then causes the dropdown list
*       to show the info about the memory block.
****************************************************************************/

void CDataList::OnRButtonDown(UINT nFlags, CPoint point) 
{
 // We can't call the On-handler OnLButtonDown because Microsoft has
 // incorrectly implemented it to simply execute the current message
 // instead of actually performing the action specified

 //     CListBox::OnLButtonDown(nFlags, point);  // note: same as "L"
        SendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELONG(point.x, point.y));

        int sel = GetCurSel();
        alloc * data = GetItemData(sel);

        for(int i= 0; i < data->count; i++)
           { /* check for within */
            if(data->pr[i].left <= point.x && point.x <= data->pr[i].right)
               { /* found it */
#if 0
                SetCapture();
#endif
                GetParent()->SendMessage(UWM_SHOW_INFO, i);
               } /* found it */
           } /* check for within */
}

void CDataList::OnRButtonUp(UINT nFlags, CPoint point) 
{
#if 0
        if(GetCapture() == this)
           { /* release it */
            ReleaseCapture();
            GetParent()->SendMessage(UWM_RELEASE_INFO);
           } /* release it */
#endif  
//      CListBox::OnLButtonUp(nFlags, point);    // note: same as "L"
        SendMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELONG(point.x, point.y));
}
