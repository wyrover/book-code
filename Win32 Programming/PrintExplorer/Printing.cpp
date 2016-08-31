// Printing.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "Printing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrinting dialog


CPrinting::CPrinting(CDC * dc, CWnd* pParent /*=NULL*/ )
        : CDialog(CPrinting::IDD, pParent)
{
        pagecount = 1;
        UpdatePageCount();
        hDC = dc;
        //{{AFX_DATA_INIT(CPrinting)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CPrinting::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPrinting)
        DDX_Control(pDX, IDC_PAGENO, c_PageNo);
        DDX_Control(pDX, IDC_ENDPAGE, c_EndPage);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrinting, CDialog)
        //{{AFX_MSG_MAP(CPrinting)
        ON_BN_CLICKED(IDC_ENDPAGE, OnEndpage)
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinting message handlers


/****************************************************************************
*                         CPrinting::UpdatePageCount
* Result: void
*       
* Effect: 
*       Updates the page count display, AND disables the EndPage() control
*       if "enough" pages have been printed
****************************************************************************/

#define PAGELIMIT 3

void CPrinting::UpdatePageCount()
    {
     CString s;
     s.Format("%d/%d", pagecount, PAGELIMIT);
     c_PageNo.SetWindowText(s);
     c_EndPage.EnableWindow(pagecount >= PAGELIMIT);
    }

/****************************************************************************
*                             CPrinting::FillPage
* Inputs:
*       int id: String ID of resource string
* Result: void
*       
* Effect: 
*       Starting with the indicated resource string, prints out the string id,
*       then higher and higher string ids until a string of length 0 is
*       returned.
****************************************************************************/

void CPrinting::FillPage(int id)
    {
     CString s;
     

     while(s.LoadString(id) > 0)
        { /* print it */
         
        } /* print it */
    }

void CPrinting::OnEndpage() 
{
 hDC->EndPage();

 pagecount++;
 UpdatePageCount();
 hDC->StartPage();
}

void CPrinting::OnCancel() 
{
 hDC->AbortDoc();
        
 CDialog::OnCancel();
}

void CPrinting::OnOK() 
{
 
        hDC->EndPage();
        hDC->EndDoc();
        
        CDialog::OnOK();
}

void CPrinting::OnDestroy() 
{
        CDialog::OnDestroy();
        
        // TODO: Add your message handler code here
        
}

BOOL CPrinting::OnInitDialog() 
{
        CDialog::OnInitDialog();
        DOCINFO di;

        di.cbSize = sizeof(di);
        di.lpszDocName = _T("Sample Document");
        di.lpszOutput = NULL;
        
        hDC->StartDoc(&di);
        hDC->StartPage();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
