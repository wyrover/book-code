// DefHeap.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "legendi.h"
#include "Legend.h"
#include "heappic.h"
#include "DefHeap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefaultHeap dialog


CDefaultHeap::CDefaultHeap(CWnd* pParent /*=NULL*/)
        : CDialog(CDefaultHeap::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDefaultHeap)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CDefaultHeap::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDefaultHeap)
        DDX_Control(pDX, IDC_INFO, c_Info);
        DDX_Control(pDX, IDC_LEGEND, c_Legend);
        DDX_Control(pDX, IDC_HEAP, c_Heap);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefaultHeap, CDialog)
        //{{AFX_MSG_MAP(CDefaultHeap)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/****************************************************************************
*                          CDefaultHeap::countheap
* Result: void
*       
* Effect: 
*       Counts the heap, then puts the values in the 
****************************************************************************/

void CDefaultHeap::countheap()
    {
     HANDLE defHeap = GetProcessHeap();
     int n = GetProcessHeaps(0, NULL);  // ask for heaps

     CString s;

    }



/////////////////////////////////////////////////////////////////////////////
// CDefaultHeap message handlers

BOOL CDefaultHeap::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        countheap();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
