// Heap.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "legendi.h"
#include "data.h"
#include "message.h"
#include "legend.h"
#include "propbut.h"
#include "infobox.h"
#include "Heap.h"
#include "Is95.h"

#include "infomsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHeap dialog


CHeap::CHeap(CWnd* pParent /*=NULL*/)
        : CDialog(CHeap::IDD, pParent)
{
        theHeap = NULL;
        //{{AFX_DATA_INIT(CHeap)
        //}}AFX_DATA_INIT
}


void CHeap::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CHeap)
        DDX_Control(pDX, IDC_FREE_9, c_Free_9);
        DDX_Control(pDX, IDC_FREE_8, c_Free_8);
        DDX_Control(pDX, IDC_FREE_7, c_Free_7);
        DDX_Control(pDX, IDC_FREE_6, c_Free_6);
        DDX_Control(pDX, IDC_FREE_5, c_Free_5);
        DDX_Control(pDX, IDC_FREE_4, c_Free_4);
        DDX_Control(pDX, IDC_FREE_30, c_Free_30);
        DDX_Control(pDX, IDC_FREE_3, c_Free_3);
        DDX_Control(pDX, IDC_FREE_29, c_Free_29);
        DDX_Control(pDX, IDC_FREE_28, c_Free_28);
        DDX_Control(pDX, IDC_FREE_27, c_Free_27);
        DDX_Control(pDX, IDC_FREE_26, c_Free_26);
        DDX_Control(pDX, IDC_FREE_25, c_Free_25);
        DDX_Control(pDX, IDC_FREE_24, c_Free_24);
        DDX_Control(pDX, IDC_FREE_23, c_Free_23);
        DDX_Control(pDX, IDC_FREE_22, c_Free_22);
        DDX_Control(pDX, IDC_FREE_21, c_Free_21);
        DDX_Control(pDX, IDC_FREE_20, c_Free_20);
        DDX_Control(pDX, IDC_FREE_2, c_Free_2);
        DDX_Control(pDX, IDC_FREE_19, c_Free_19);
        DDX_Control(pDX, IDC_FREE_18, c_Free_18);
        DDX_Control(pDX, IDC_FREE_17, c_Free_17);
        DDX_Control(pDX, IDC_FREE_16, c_Free_16);
        DDX_Control(pDX, IDC_FREE_15, c_Free_15);
        DDX_Control(pDX, IDC_FREE_14, c_Free_14);
        DDX_Control(pDX, IDC_FREE_13, c_Free_13);
        DDX_Control(pDX, IDC_FREE_12, c_Free_12);
        DDX_Control(pDX, IDC_FREE_11, c_Free_11);
        DDX_Control(pDX, IDC_FREE_10, c_Free_10);
        DDX_Control(pDX, IDC_INFO, c_Info);
        DDX_Control(pDX, IDC_FREE_1, c_Free_1);
        DDX_Control(pDX, IDC_LEGEND, c_Legend);
        DDX_Control(pDX, IDC_COMPACT, c_HeapCompact);
        DDX_Control(pDX, IDC_10, c_10);
        DDX_Control(pDX, IDC_PAGES, c_Pages);
        DDX_Control(pDX, IDC_PAGE1, c_Page1);
        DDX_Control(pDX, IDC_PAGE2, c_Page2);
        DDX_Control(pDX, IDC_ALLOCATOR, c_Allocator);
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_DATA, c_Data);
        DDX_Control(pDX, IDC_DESTROY, c_HeapDestroy);
        DDX_Control(pDX, IDC_CREATE, c_HeapCreate);
        DDX_Control(pDX, IDC_FREE, c_HeapFree);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeap, CDialog)
        //{{AFX_MSG_MAP(CHeap)
        ON_BN_CLICKED(IDC_10, On10)
        ON_BN_CLICKED(IDC_100, On100)
        ON_BN_CLICKED(IDC_20, On20)
        ON_BN_CLICKED(IDC_30, On30)
        ON_BN_CLICKED(IDC_40, On40)
        ON_BN_CLICKED(IDC_50, On50)
        ON_BN_CLICKED(IDC_60, On60)
        ON_BN_CLICKED(IDC_70, On70)
        ON_BN_CLICKED(IDC_80, On80)
        ON_BN_CLICKED(IDC_90, On90)
        ON_BN_CLICKED(IDC_DESTROY, OnDestroyHeap)
        ON_BN_CLICKED(IDC_CREATE, OnCreateHeap)
        ON_LBN_SELCHANGE(IDC_DATA, OnSelchangeData)
        ON_WM_DESTROY()
        ON_CBN_SELENDCANCEL(IDC_INFO, OnSelendcancelInfo)
        ON_CBN_SELENDOK(IDC_INFO, OnSelendokInfo)
        ON_CBN_SELCHANGE(IDC_INFO, OnSelchangeInfo)
        //}}AFX_MSG_MAP
        ON_COMMAND_RANGE(IDC_FREE_1, IDC_FREE_30, OnFreeButton)
        ON_MESSAGE(UWM_SHOW_INFO, OnShowInfo)
        ON_MESSAGE(UWM_RELEASE_INFO, OnReleaseInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeap message handlers

void CHeap::On10() 
{
 allocate(10);
        
}

void CHeap::On100() 
{
 allocate(100);
        
}

void CHeap::On20() 
{
 allocate(20);
        
}

void CHeap::On30() 
{
 allocate(30);
        
}

void CHeap::On40() 
{
 allocate(40);
        
}

void CHeap::On50() 
{
 allocate(50);
        
}

void CHeap::On60() 
{
 allocate(60);
        
}

void CHeap::On70() 
{
 allocate(70);
        
}

void CHeap::On80() 
{
 allocate(80);
        
}

void CHeap::On90() 
{
 allocate(90);
        
}

/****************************************************************************
*                          CHeap::create_heap_entry
* Result: void
*       
* Effect: 
*       Adds an entry to the list box which contains all of the structure
*       information about the heap
****************************************************************************/

void CHeap::create_heap_entry(LPVOID p)
    {
     alloc * data = new alloc;
     int i;
     PROCESS_HEAP_ENTRY n;
     SYSTEM_INFO info;

     GetSystemInfo(&info);

     n.lpData = NULL;
     n.wFlags = PROCESS_HEAP_REGION;
     HeapWalk(theHeap, &n);
     data->base = (LPVOID)((DWORD)n.lpData & ~(DWORD)(pagesize-1));
     data->quantum = factor;
     data->range = MYHEAP_LIMIT;
     data->pages = pages;
     data->pagesize = pagesize;


     data->lasterr = result;
     data->here = p;
     i = 0;
     n.lpData = NULL;
     n.wFlags = 0;
     

     while(HeapWalk(theHeap, &n))
        { /* do heap walk */
         data->ph[i] = n;
         i++;
         data->count++;
        } /* do heap walk */

     data->seq = c_Data.AddString(data);
     c_Data.SetCurSel(data->seq);
     showSelection(data->seq);
    }

/****************************************************************************
*                               CHeap::allocate
* Inputs:
*       int size: Size to allocate
* Result: void
*       
* Effect: 
*       Attempts to allocate the storage
****************************************************************************/

void CHeap::allocate(int size)
    {
     LPVOID data;
     if(theHeap == NULL)
        return;

     data = HeapAlloc(theHeap, 0, size * factor);
     if(data == NULL)
        { /* error */
         result = GetLastError();
         MessageBeep(0);
        } /* error */
     else
        { /* ok */
         result = 0;
        } /* ok */
     create_heap_entry(data);
     heap_to_controls();

    }

/****************************************************************************
*                           CHeap::enable_allocator
* Inputs:
*       BOOL mode: mode to use, TRUE or FALSE.  TRUE enables all the controls
*               that are valid when a heap is active; FALSE disables the
*               controls.  The no-heap controls are complementarily enabled.
* Result: void
*       
* Effect: 
*       Enables the allocation controls
****************************************************************************/

void CHeap::enable_allocator(BOOL mode)
    {
     for(int i = IDC_10; i <= IDC_100; i++)
        { /* disable controls */
         GetDlgItem(i)->EnableWindow(mode);
        } /* disable controls */

     if(pages == 2)
        c_10.EnableWindow(FALSE);

     c_Allocator.EnableWindow(mode);
     c_Page1.EnableWindow(!mode);
     c_Page2.EnableWindow(!mode);
     c_Pages.EnableWindow(!mode);
     c_HeapCreate.EnableWindow(!mode);
     c_HeapDestroy.EnableWindow(mode);
     c_HeapCompact.EnableWindow(mode);
     c_Data.EnableWindow(mode);
     c_Info.EnableWindow(mode);
    }


/****************************************************************************
*                              CHeap::setMessage
* Inputs:
*       DWORD result: Result to display
* Result: void
*       
* Effect: 
*       Puts the string representing the message in the c_Result box
****************************************************************************/

void CHeap::setMessage(DWORD result)
    {
     if(result == 0)
        { /* no message */
         c_Result.SetWindowText(NULL);
         return;
        } /* no message */

     LPTSTR msg;
     if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | 
                   FORMAT_MESSAGE_ALLOCATE_BUFFER,
                   NULL, 
                   result, 0, 
                   (LPTSTR)&msg,
                   0,
                   NULL) != 0)
        { /* success */
         c_Result.SetWindowText(msg);
         LocalFree(msg);
        } /* success */
     else
        { /* couldn't report error */
         c_Result.SetWindowText(_T("?"));
        } /* couldn't report error */
     
    }

/****************************************************************************
*                              CHeap::enable_Free
* Inputs:
*       BOOL mode: 
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CHeap::enable_Free(BOOL mode)
    {
     if(!mode)
        { /* disable */
         for(int i = IDC_FREE_1; i <= IDC_FREE_30; i++)
            { /* enable controls */
             GetDlgItem(i)->EnableWindow(mode);
            } /* enable controls */
         c_HeapFree.EnableWindow(mode);
         return;
        } /* disable */

     alloc * data = c_Data.GetItemData(c_Data.GetCurSel());
     BOOL any = FALSE;

     for(int i = 1; i < data->count; i++)
        { /* enable each */
         CButton * B = (CButton *)GetDlgItem(IDC_FREE_1 + (i - 1));
         if(data->ph[i].wFlags == 0)
            { /* disable */
             B->EnableWindow(FALSE);
             B->ShowWindow(SW_SHOW);
             any = TRUE;
            } /* disable */
         else
            { /* enable */
             B->EnableWindow(TRUE);
             ::SetWindowLong(B->m_hWnd, GWL_USERDATA, (LPARAM)data->ph[i].lpData);
            } /* enable */
        } /* enable each */
     for(; i <= (IDC_FREE_30 - IDC_FREE_1) + 1; i++)
        { /* hide remainder */
         GetDlgItem(IDC_FREE_1 + (i - 1))->ShowWindow(SW_HIDE);
        } /* hide remainder */
     if(any)
        c_HeapFree.EnableWindow(TRUE);
    }

/****************************************************************************
*                             enable_Free_matrix
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CHeap::enable_Free_matrix()
    {
     int count = c_Data.GetCount();
     int sel = c_Data.GetCurSel();
     if(sel != CB_ERR && sel == count - 1)
        { /* enable free */
         enable_Free(TRUE);
        } /* enable free */
     else
        { /* disable free */
         enable_Free(FALSE);
        } /* disable free */
     
    }

/****************************************************************************
*                           CHeap::heap_to_controls
* Result: void
*       
* Effect: 
*       Transfers the state of the heap to the controls
****************************************************************************/

void CHeap::heap_to_controls()
    {
     if(theHeap == NULL)
        { /* no heap */
         enable_allocator(FALSE);
        } /* no heap */
     else
        { /* has heap */
         enable_allocator(TRUE);
        } /* has heap */


     enable_Free_matrix();
    }

BOOL CHeap::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        SYSTEM_INFO info;

        GetSystemInfo(&info);

        pagesize = info.dwPageSize;

        CheckRadioButton(IDC_PAGE1, IDC_PAGE2, IDC_PAGE1);

        LegendInfo ** li = CDataList::GetLegend();
        for(int i = 0; li[i] != NULL; i++)
           { /* load legend */
            c_Legend.AddString(li[i]);
           } /* load legend */
        c_Legend.SetCurSel(0);

        heap_to_controls();

        if(Is95())
           { /* not available */
            AfxMessageBox(IDS_NOFEATURE);
            SendMessage(WM_CLOSE);
           } /* not available */

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CHeap::OnDestroyHeap() 
{
 if(HeapDestroy(theHeap))
    { /* success */
     theHeap = NULL;
     result = 0;
     c_Info.ResetContent();
    c_Data.ResetContent();
    } /* success */
 else
    { /* failure */
     result = GetLastError();
    } /* failure */
 setMessage(result);
 heap_to_controls();
        
}

void CHeap::OnCreateHeap() 
{
 int size = pagesize;

 pages = 1;

 if(c_Page2.GetCheck())
    pages = 2;

 size *= pages;
 theHeap = HeapCreate(0, size, size);

 factor = pagesize / MYHEAP_LIMIT;  // units per page
        
 if(theHeap == NULL)
    result = GetLastError();
 else
    { /* success */
     result = 0;
     
     create_heap_entry(NULL);
    } /* success */

 heap_to_controls();
        
}


/****************************************************************************
*                               CHeap::showInfo
* Inputs:
*       PROCESS_HEAP_ENTRY * ph:
* Result: int
*       Total memory allocation
* Effect: 
*       ²
****************************************************************************/

int CHeap::showInfo(int i, PROCESS_HEAP_ENTRY * ph)
    {
     CString s;
     CString fmt;

     if(ph->wFlags == PROCESS_HEAP_REGION)
        { /* header */
         s.Format(_T("%d: PROCESS_HEAP_REGION"), i);
        } /* header */
     else
     if(ph->wFlags == 0)
        { /* free space */
         fmt.LoadString(IDS_FREESPACE);
         s.Format(fmt, i);
        } /* free space */
     else
        { /* busy space */
         fmt.LoadString(IDS_ALLOCSPACE);
         s.Format(fmt, i);
        } /* busy space */
     c_Info.AddString(s, i);

     s.Format(_T("    lpData %08x"), ph->lpData);
     c_Info.AddString(s, i);

     s.Format(_T("    cbData %d"), ph->cbData);
     c_Info.AddString(s, i);

     s.Format(_T("    cbOverhead %d"), ph->cbOverhead);
     c_Info.AddString(s, i);

     s.Format(_T("    iRegionIndex %d"), (DWORD)ph->iRegionIndex);
     c_Info.AddString(s, i);

     s.Format(_T("    wFlags %04x"), ph->wFlags);
     c_Info.AddString(s, i);
     if(ph->wFlags & PROCESS_HEAP_REGION)
        { /* region */
         s.Format(_T("        PROCESS_HEAP_REGION"));
         c_Info.AddString(s, i);
        } /* region */
     if(ph->wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE)
        { /* uncommitted */
         s.Format(_T("        PROCESS_HEAP_UNCOMITTED_RANGE"));
         c_Info.AddString(s, i);
        } /* uncommitted */
     if(ph->wFlags & PROCESS_HEAP_ENTRY_BUSY)
        { /* busy */
         s.Format(_T("        PROCESS_HEAP_ENTRY_BUSY"));
         c_Info.AddString(s, i);
        } /* busy */
     if(ph->wFlags & PROCESS_HEAP_ENTRY_MOVEABLE)
        { /* moveable */
         s.Format(_T("        PROCESS_HEAP_ENTRY_MOVEABLE"));
         c_Info.AddString(s, i);
        } /* moveable */
     if(ph->wFlags & PROCESS_HEAP_ENTRY_DDESHARE)
        { /* DDEshare */
         s.Format(_T("        PROCESS_HEAP_ENTRY_DDESHARE"));
         c_Info.AddString(s, i);
        } /* DDEshare */

     if( (ph->wFlags & 
        (PROCESS_HEAP_ENTRY_BUSY | PROCESS_HEAP_ENTRY_MOVEABLE))
          == (PROCESS_HEAP_ENTRY_BUSY | PROCESS_HEAP_ENTRY_MOVEABLE))
        { /* Block */
         s.Format(_T("    Block:"));
         c_Info.AddString(s, i);
         s.Format(_T("      hMem %08x"), ph->Block.hMem);
         c_Info.AddString(s, i);
        } /* Block */
     else
     if (ph->wFlags & PROCESS_HEAP_REGION)
        { /* Region */
         s.Format(_T("    Region:"));
         c_Info.AddString(s, i);

         s.Format(_T("      dwCommittedSize %d"), ph->Region.dwCommittedSize);
         c_Info.AddString(s, i);
         
         s.Format(_T("      dwUnCommitedSize %d"),ph->Region.dwUnCommittedSize);
         c_Info.AddString(s, i);

         s.Format(_T("      lpFirstBlock %08x"), ph->Region.lpFirstBlock);
         c_Info.AddString(s, i);

         s.Format(_T("      lpLastBlock %08x"), ph->Region.lpLastBlock);
         c_Info.AddString(s, i);

        } /* Region */
     c_Info.AddString(_T("--------------------------------"), i);
     return ph->cbData + ph->cbOverhead;
    }

/****************************************************************************
*                            CHeap::showSelection
* Inputs:
*       int i: Index of selection
* Result: void
*       
* Effect: 
*       Shows the selection
****************************************************************************/

void CHeap::showSelection(int sel)
    {
     alloc * data = (alloc *)(c_Data.GetItemData(sel));

     setMessage(data->lasterr);
         
     CString s;
     c_Info.ResetContent();

     CString fmt;
     fmt.LoadString(IDS_SUMMARY);
     DWORD useful = 0;
     DWORD total = 0;
     DWORD allocated = 0;
     DWORD free = 0;
     DWORD largest_free = 0;

     for(int i = 0; i < data->count; i++)
        { /* build string */
         total += showInfo(i, &data->ph[i]);
         if(data->ph[i].wFlags == 0)
            { /* free */
             free += data->ph[i].cbData;
             useful += data->ph[i].cbData;
             largest_free = max(largest_free, data->ph[i].cbData);
            } /* free */
         else
         if(data->ph[i].wFlags & PROCESS_HEAP_REGION)
            { /* region */
             // TODO: something here
            } /* region */
         else
            { /* allocated */
             allocated +=data->ph[i].cbData;
             useful +=data->ph[i].cbData;
            } /* allocated */
        } /* build string */

     // Heap size: %1
     // Free space: %2
     // Allocated space: %3
     // Useful space: %4

     s.FormatMessage(fmt, total, free, allocated, useful);
     c_Info.InsertString(0, s);
     c_Info.SetCurSel(0);
    }


void CHeap::OnSelchangeData() 
{

 showSelection(c_Data.GetCurSel());
 enable_Free_matrix();
}

void CHeap::OnDestroy() 
{
        CDialog::OnDestroy();
        
        if(theHeap != 0)
           { /* delete heap */
            HeapDestroy(theHeap);
           } /* delete heap */
        
}

/****************************************************************************
*                                CHeap::OnFreeButton
* Inputs:
*       UINT id: ID of control
* Result: void
*       
* Effect: 
*       Deletes the objec associated with the control
****************************************************************************/

void CHeap::OnFreeButton(UINT id)
    {
     CWnd * wnd = GetDlgItem(id);
     LPVOID addr = (LPVOID)::GetWindowLong(wnd->m_hWnd, GWL_USERDATA);
     HeapFree(theHeap, 0, addr);
     create_heap_entry(NULL);
     heap_to_controls();
    }

void CHeap::OnSelendcancelInfo() 
{
 c_Info.SetCurSel(0);
}

void CHeap::OnSelendokInfo() 
{
 c_Info.SetCurSel(0);
}

/****************************************************************************
*                              CHeap::OnShowInfo
* Inputs:
*       WPARAM id: ID of control
*       LPARAM : ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Drops down info for the selected region of data
****************************************************************************/

LRESULT CHeap::OnShowInfo(WPARAM id, LPARAM)
    {
     if(id >= IDC_FREE_1)
        id = (id - IDC_FREE_1) + 1;

     int index = c_Info.FindItem(id);
     c_Info.SetCurSel(index);
     c_Info.ShowDropDown(TRUE);

     CString s;
     s.Format(_T("%d : %d"), id, index);
     c_Result.SetWindowText(s);

     return 0;
    }

/****************************************************************************
*                              CHeap::OnReleaseInfo
* Inputs:
*       WPARAM : ignored
*       LPARAM : ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Hides dropdown of info for the selected region of data
****************************************************************************/

LRESULT CHeap::OnReleaseInfo(WPARAM, LPARAM)
    {
     c_Info.ShowDropDown(FALSE);

     c_Result.SetWindowText(_T(""));
     return 0;
    }

void CHeap::OnSelchangeInfo() 
{
#if 0
 int index = c_Info.GetCurSel();
 DWORD item = c_Info.GetItemData(index);

 CString s;
 s.Format("%08lx", item);
 c_Result.SetWindowText(s);
 c_Result.UpdateWindow();
#endif  
}
