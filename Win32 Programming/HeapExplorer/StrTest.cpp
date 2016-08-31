// StrTest.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "mystring.h"
#include "strdisp.h"
#include "legendi.h"
#include "legend.h"
#include "heappic.h"
#include "memmap.h"
#include "StrTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringTest dialog


CStringTest::CStringTest(CWnd* pParent /*=NULL*/)
        : CDialog(CStringTest::IDD, pParent)
{
        //{{AFX_DATA_INIT(CStringTest)
        //}}AFX_DATA_INIT
}


void CStringTest::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CStringTest)
        DDX_Control(pDX, IDC_LEGEND, c_Legend);
        DDX_Control(pDX, IDC_AUTODEALLOC, c_Dealloc);
        DDX_Control(pDX, IDC_COMPACT, c_Compact);
        DDX_Control(pDX, IDC_LAYOUT, c_Layout);
        DDX_Control(pDX, IDC_INPUT, c_Input);
        DDX_Control(pDX, IDC_STRINGS, c_Strings);
        DDX_Control(pDX, IDC_INFO, c_Info);
        DDX_Control(pDX, IDC_DELETE, c_Delete);
        DDX_Control(pDX, IDC_ADD, c_Add);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringTest, CDialog)
        //{{AFX_MSG_MAP(CStringTest)
        ON_BN_CLICKED(IDC_ADD, OnAdd)
        ON_BN_CLICKED(IDC_DELETE, OnDelete)
        ON_CBN_SELENDCANCEL(IDC_INFO, OnSelendcancelInfo)
        ON_CBN_SELENDOK(IDC_INFO, OnSelendokInfo)
        ON_CBN_SELENDOK(IDC_STRINGS, OnSelendokStrings)
        ON_EN_CHANGE(IDC_INPUT, OnChangeInput)
        ON_BN_CLICKED(IDC_COMPACT, OnCompact)
        ON_BN_CLICKED(IDC_AUTODEALLOC, OnAutodealloc)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringTest message handlers

/****************************************************************************
*                          CStringTest::inputExists
* Result: BOOL
*       TRUE if string is already in combo box, or string is empty
*       FALSE if string not found in combo box
****************************************************************************/

BOOL CStringTest::inputExists()
    {
     CString s;

     c_Input.GetWindowText(s);

     if(s.GetLength() == 0)
        return TRUE; // empty string always "exists"

     return (c_Strings.FindStringExact(-1, s) != CB_ERR);
    }
        

void CStringTest::OnAdd() 
{
 if(!inputExists())
    { /* not found */
     CString s;
     c_Input.GetWindowText(s);

     CMyData * p = new CMyData;
     p->setString(s);

     int index = c_Strings.AddString(p);
     c_Input.SetWindowText(_T(""));
     c_Strings.SetCurSel(index);
    } /* not found */
 heap_to_display();
 c_Layout.InvalidateRect(NULL, TRUE);
 c_Input.SetFocus();
}

void CStringTest::OnDelete() 
{
 int index = c_Strings.GetCurSel();
 if(index == CB_ERR)
    return;

 CMyData * p = (CMyData *)c_Strings.GetItemData(index);
 delete p;
 c_Strings.DeleteString(index);
 c_Strings.SetCurSel(0); 
 heap_to_display();
 c_Layout.InvalidateRect(NULL, TRUE);
}

/****************************************************************************
*                        CStringTest::heap_to_display
* Result: void
*       
* Effect: 
*       Puts heap info in the display box
****************************************************************************/

void CStringTest::heap_to_display()
    {
     PROCESS_HEAP_ENTRY e;
     HANDLE heap = CMyData::getHeapHandle();

     c_Info.ResetContent();

     enableControls();

     if(heap == NULL)
        { /* no heap */
         CString s;
         s.LoadString(IDS_NO_HEAP);
         c_Info.AddString(s);
         return;
        } /* no heap */

     e.lpData = NULL;

     while(HeapWalk(heap, &e))
        { /* scan heap */
         int index;

         if(e.wFlags == 0)
            { /* free */
             CString s;
             s.FormatMessage(IDS_MT_FREE_SIZE, e.cbOverhead, e.cbData, 
                                                        e.lpData);
             index = c_Info.AddString(s);
             c_Info.SetItemData(index, (LPARAM)e.lpData);
            } /* free */
         else
         if(e.wFlags & PROCESS_HEAP_REGION)
            { /* region header */
             CString s;
             s.FormatMessage(IDS_MT_HEADER, e.cbOverhead, e.cbData, e.lpData);
             index = c_Info.AddString(s);
             c_Info.SetItemData(index, (LPARAM)e.lpData);
            } /* region header */
         else
         if(e.wFlags & PROCESS_HEAP_ENTRY_BUSY)
            { /* occupied */
             CString s;
             s.FormatMessage(IDS_MT_DATA, e.cbOverhead, e.cbData, e.lpData);
             index = c_Info.AddString(s);
             c_Info.SetItemData(index, (LPARAM)e.lpData);
            } /* occupied */
         else
         if(e.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE)
            { /* uncommitted */
             CString s;
             s.FormatMessage(IDS_MT_UNCOMMITTED, e.cbOverhead, e.cbData,
                                                e.lpData);
             index = c_Info.AddString(s);
             c_Info.SetItemData(index, (LPARAM)e.lpData);
            } /* uncommitted */
        } /* scan heap */

    }

BOOL CStringTest::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        heap_to_display();
        c_Dealloc.SetCheck(CMyData::getAutoDeallocate());
        
        LegendInfo ** legends = CMemoryMap::getLegend();

        for(int i = 0; legends[i] != NULL; i++)
           c_Legend.AddString(legends[i]);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CStringTest::OnSelendcancelInfo() 
{
        // TODO: Add your control notification handler code here
        
}

void CStringTest::OnSelendokInfo() 
{
 syncStringsToInfo();
 enableControls();
}

void CStringTest::OnSelendokStrings() 
{
 syncInfoToStrings();
 enableControls();
}

/****************************************************************************
*                               enableControls
* Inputs:
*       ²
* Result: void
*       
* Effect: 
*       Enables the various controls
****************************************************************************/

void CStringTest::enableControls()
    {
     c_Add.EnableWindow(!inputExists());
     c_Delete.EnableWindow(c_Strings.GetCurSel() != CB_ERR);
     c_Layout.EnableWindow(CMyData::getHeapHandle() == NULL);
     c_Compact.EnableWindow(CMyData::getHeapHandle() != NULL);
    }

void CStringTest::OnChangeInput() 
{
 enableControls();
        
}

/****************************************************************************
*                       CStringTest::syncInfoToStrings
* Result: void
*       
* Effect: 
*       Takes the item in the Strings window and finds the matching item
*       in the info window
****************************************************************************/

void CStringTest::syncInfoToStrings()
    {
     DWORD source;
     DWORD item;

     source = (DWORD)c_Strings.GetItemData(c_Strings.GetCurSel());

     int count = c_Info.GetCount();
     for(int i = 0; i < count; i++)
        { /* try select */
         item = (DWORD)c_Info.GetItemData(i);
         if(item == source)
            { /* found it */
             c_Info.SetCurSel(i);
             enableControls();
             return;
            } /* found it */
        } /* try select */
     c_Info.SetCurSel(-1);
    }

/****************************************************************************
*                       CStringTest::syncStringsToInfo
* Result: void
*       
* Effect: 
*       Synchronizes the Strings entry to the Info box
****************************************************************************/

void CStringTest::syncStringsToInfo()
    {
     DWORD source = (DWORD)c_Info.GetItemData(c_Info.GetCurSel());
     DWORD item;
     int count = c_Strings.GetCount();
     for(int i = 0; i < count; i++)
        { /* try select */
         item = (DWORD)c_Strings.GetItemData(i);
         if(item == source)
            { /* found it */
             c_Strings.SetCurSel(i);
             enableControls();
             return;
            } /* found it */
        } /* try select */
     c_Strings.SetCurSel(-1);
     
    }

void CStringTest::OnCompact() 
{
 HANDLE heap = CMyData::getHeapHandle();
 if(heap == NULL)
    return;
 HeapCompact(heap, 0);
        
}

void CStringTest::OnAutodealloc() 
{
 CMyData::setAutoDeallocate(c_Dealloc.GetCheck());
        
}
