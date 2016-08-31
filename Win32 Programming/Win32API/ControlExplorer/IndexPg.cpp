// IndexPg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "IndexPg.h"

#include "tinybutt.h"
#include "IntEdit.h"
#include "HexDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static ixlist entries[] = {
     //---------------------------------------------------
     {0, 0, 0}  // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CIndexPage property page

IMPLEMENT_DYNCREATE(CIndexPage, CPropertyPage)

ixlist * CIndexPage::getIndexTable()
   {
    return entries;
   }

CIndexPage::CIndexPage() : CPropertyPage(CIndexPage::IDD)
{
        //{{AFX_DATA_INIT(CIndexPage)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CIndexPage::~CIndexPage()
{
}

void CIndexPage::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CIndexPage)
        DDX_Control(pDX, IDC_INDEX, c_Index);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexPage, CPropertyPage)
        //{{AFX_MSG_MAP(CIndexPage)
        ON_LBN_DBLCLK(IDC_INDEX, OnDblclkIndex)
        ON_BN_CLICKED(IDC_SHOW, OnShow)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexPage message handlers


/****************************************************************************
*                            CIndexPage::gotoPage
* Inputs:
*       int page: Page id
*       int ctlid: Control id within page, or 0 for no focus change
* Result: void
*       
* Effect: 
*       Switches to the specified page
****************************************************************************/

void CIndexPage::gotoPage(int page, int ctlid)
{
 CPropertySheet * sheet = (CPropertySheet *)GetParent();

 int count = sheet->GetPageCount(); // total number of pages

 for(int i = 0; i < count; i++)
    { /* check page */
     CPropertyPage * p = sheet->GetPage(i);
     if(p->IsKindOf(RUNTIME_CLASS(CIndexablePage)))
        { /* has it */
         CIndexablePage * op = (CIndexablePage *)p;
         if(op->id == page)
            { /* found it */
             if(ctlid != 0)
                { /* set focus */
                 op->focus = ctlid;
                } /* set focus */
             sheet->SetActivePage(i);
             return;
            } /* found it */
        } /* has it */
    } /* check page */
}

void CIndexPage::OnDblclkIndex() 
{
 DWORD target = c_Index.GetItemData(c_Index.GetCurSel());
 gotoPage(LOWORD(target), HIWORD(target));
        
}

BOOL CIndexPage::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        ixlist * table = getIndexTable();

        for(int i = 0; table[i].id != 0; i++)
           { /* load table */
            CString s;
            s.LoadString(table[i].id);
            int ix = c_Index.AddString(s);
            if(ix != LB_ERR)
               c_Index.SetItemData(ix, MAKELONG(table[i].page, table[i].ctlid));
           } /* load table */
        
        c_Index.SetCurSel(0);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CIndexPage::OnShow() 
{
 DWORD target = c_Index.GetItemData(c_Index.GetCurSel());
 gotoPage(LOWORD(target), HIWORD(target));
}
