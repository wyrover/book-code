// StockFnt.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "StockFnt.h"
#include "toclip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStockFont dialog


CStockFont::CStockFont(CWnd* pParent /*=NULL*/)
        : CDialog(CStockFont::IDD, pParent)
{
        //{{AFX_DATA_INIT(CStockFont)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CStockFont::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CStockFont)
        DDX_Control(pDX, IDC_SYSTEM_FONT, c_SYSTEM_FONT);
        DDX_Control(pDX, IDC_SYSTEM_FIXED_FONT, c_SYSTEM_FIXED_FONT);
        DDX_Control(pDX, IDC_OEM_FIXED_FONT, c_OEM_FIXED_FONT);
        DDX_Control(pDX, IDC_DEVICE_DEFAULT_FONT, c_DEVICE_DEFAULT_FONT);
        DDX_Control(pDX, IDC_ANSI_VAR_FONT, c_ANSI_VAR_FONT);
        DDX_Control(pDX, IDC_ANSI_FIXED_FONT, c_ANSI_FIXED_FONT);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStockFont, CDialog)
        //{{AFX_MSG_MAP(CStockFont)
        ON_WM_CTLCOLOR()
        ON_BN_CLICKED(IDC_ANSI_FIXED_FONT, OnAnsiFixedFont)
        ON_BN_CLICKED(IDC_ANSI_VAR_FONT, OnAnsiVarFont)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_BN_CLICKED(IDC_DEVICE_DEFAULT_FONT, OnDeviceDefaultFont)
        ON_BN_CLICKED(IDC_OEM_FIXED_FONT, OnOemFixedFont)
        ON_BN_CLICKED(IDC_SYSTEM_FIXED_FONT, OnSystemFixedFont)
        ON_BN_CLICKED(IDC_SYSTEM_FONT, OnSystemFont)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStockFont message handlers

HBRUSH CStockFont::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        if(pWnd->m_hWnd == c_Sample.m_hWnd)
            return (HBRUSH)::GetStockObject(WHITE_BRUSH);

        HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        

        return hbr;
}

BOOL CStockFont::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CheckRadioButton(IDC_ANSI_FIXED_FONT, IDC_SYSTEM_FONT, 
                                                        IDC_SYSTEM_FONT);
        OnSystemFont();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}


/****************************************************************************
*                            CStockFont::setSample
* Inputs:
*       CButton & caption: Caption for contents
*       int fontid: Font ID value for stock object
* Result: void
*       
* Effect: 
*       Selects the stock font into the static control.  Uses the caption
*       from the 'caption' parameter as the contents
****************************************************************************/

void CStockFont::setSample(CButton & caption, int fontid)
    {
     CString s;
     caption.GetWindowText(s);

     CString extra;
     extra.LoadString(IDS_CHARS_HIGH);
     s += _T("   ");
     s += extra;

     CFont f;
     f.CreateStockObject(fontid);
     c_Sample.SetFont(&f);
     c_Sample.SetWindowText(s);
    }


void CStockFont::OnAnsiFixedFont() 
{
 setSample(c_ANSI_FIXED_FONT, ANSI_FIXED_FONT);
}

void CStockFont::OnAnsiVarFont() 
{
 setSample(c_ANSI_VAR_FONT, ANSI_VAR_FONT);
}

void CStockFont::OnCopy() 
{
 toClipboard(&c_Sample, FALSE);
}

void CStockFont::OnDeviceDefaultFont() 
{
 setSample(c_DEVICE_DEFAULT_FONT, DEVICE_DEFAULT_FONT);
}

void CStockFont::OnOemFixedFont() 
{
 setSample(c_OEM_FIXED_FONT, OEM_FIXED_FONT);   
}

void CStockFont::OnSystemFixedFont() 
{
 setSample(c_SYSTEM_FIXED_FONT, SYSTEM_FIXED_FONT);
}

void CStockFont::OnSystemFont() 
{
 setSample(c_SYSTEM_FONT, SYSTEM_FONT); 
}
