// CPlace.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "CPlace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterPlacement dialog


CCharacterPlacement::CCharacterPlacement(CWnd* pParent /*=NULL*/)
        : CDialog(CCharacterPlacement::IDD, pParent)
{
        //{{AFX_DATA_INIT(CCharacterPlacement)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CCharacterPlacement::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CCharacterPlacement)
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        DDX_Control(pDX, IDC_GCP_MAXEXTENT, c_GCP_MaxExtent);
        DDX_Control(pDX, IDC_MAXEXTENT, c_MaxExtent);
        DDX_Control(pDX, IDC_LPSTRING, c_lpString);
        DDX_Control(pDX, IDC_C_MAXEXTENT, c_c_MaxExtent);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharacterPlacement, CDialog)
        //{{AFX_MSG_MAP(CCharacterPlacement)
        ON_BN_CLICKED(IDC_FONT, OnFont)
        ON_BN_CLICKED(IDC_GCP_MAXEXTENT, OnGcpMaxextent)
        ON_BN_CLICKED(IDC_GETCHARPLACEMENT, OnGetcharplacement)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterPlacement message handlers

void CCharacterPlacement::OnFont() 
{
 CFont * oldFont;
 LOGFONT lf;

 memset(&lf, 0, sizeof(LOGFONT));
 oldFont = c_Sample.GetFont();
 oldFont->GetObject(sizeof(LOGFONT), &lf);

 CFontDialog fd(&lf);
 if(fd.DoModal())
    { /* successful choice */
     CFont newFont;
     if(newFont.CreateFontIndirect(&lf))
        { /* successful creation */
         // ... do something with font
         c_Sample.SetFont(&newFont);
         newFont.Detach();
         oldFont->DeleteObject();
        } /* successful creation */
    } /* successful choice */
     
        
}

/****************************************************************************
*                      CCharacterPlacement::enableExtent
* Inputs:
*       BOOL mode: Mode to use
* Result: void
*       
* Effect: 
*       Enables/disables the MAXEXTENT values
****************************************************************************/

void CCharacterPlacement::enableExtent(BOOL mode)
    {
     c_MaxExtent.ShowWindow(mode ? SW_SHOW : SW_HIDE);
     c_c_MaxExtent.EnableWindow(mode);
    }


void CCharacterPlacement::OnGcpMaxextent() 
{
 enableExtent(c_GCP_MaxExtent.GetCheck());
}

BOOL CCharacterPlacement::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        enableExtent(c_GCP_MaxExtent.GetCheck());
        
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CCharacterPlacement::OnGetcharplacement() 
{
 DWORD result;
 CString s;
 int maxextent;
 DWORD flags = 0;
 LPGCP_RESULTS results = NULL;

 CClientDC dc(this);

 c_lpString.GetWindowText(s);
 maxextent = GetDlgItemInt(IDC_MAXEXTENT);

 // TODO: Read flags from the controls...

 result = ::GetCharacterPlacement(dc.m_hDC, s, lstrlen(s), maxextent,
                results, flags);
        
}
