// BtnMsg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "tinybutt.h"
#include "BtnMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonMessages dialog


/****************************************************************************
*                        CButtonMessages::getButtonID
* Inputs:
*       CButton * button: Button window
* Result: CString
*       ID of button
****************************************************************************/

CString CButtonMessages::getButtonID(CButton * button)
    {
     CString s;

     if(button->m_hWnd == c_Button1->m_hWnd)
        s.LoadString(IDS_MSG_BUTTON1);
     else
     if(button->m_hWnd == c_Button2->m_hWnd)
        s.LoadString(IDS_MSG_BUTTON2);
     else
     if(button->m_hWnd == c_Button3->m_hWnd)
        s.LoadString(IDS_MSG_BUTTON3);

     return s;
    }

CButtonMessages::CButtonMessages(CWnd* pParent /*=NULL*/)
        : CDialog(CButtonMessages::IDD, pParent)
{
        //{{AFX_DATA_INIT(CButtonMessages)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CButtonMessages::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CButtonMessages)
        DDX_Control(pDX, IDC_STAVEVAL, c_StateVal);
        DDX_Control(pDX, IDC_BUTTON3, c_Button3Select);
        DDX_Control(pDX, IDC_BUTTON2, c_Button2Select);
        DDX_Control(pDX, IDC_BUTTON1, c_Button1Select);
        DDX_Control(pDX, IDC_SETSTYLE, c_SetStyle);
        DDX_Control(pDX, IDC_SETSTATE, c_SetState);
        DDX_Control(pDX, IDC_SETIMAGE, c_SetImage);
        DDX_Control(pDX, IDC_SETCHECK, c_SetCheck);
        DDX_Control(pDX, IDC_GETSTATE, c_GetState);
        DDX_Control(pDX, IDC_GETIMAGE, c_GetImage);
        DDX_Control(pDX, IDC_GETCHECK, c_GetCheck);
        DDX_Control(pDX, IDC_CLICK, c_Click);
        DDX_Control(pDX, IDC_2, c_2);
        DDX_Control(pDX, IDC_1, c_1);
        DDX_Control(pDX, IDC_0, c_0);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CButtonMessages, CDialog)
        //{{AFX_MSG_MAP(CButtonMessages)
        ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
        ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
        ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
        ON_BN_CLICKED(IDC_CLICK, OnClick)
        ON_BN_CLICKED(IDC_GETCHECK, OnGetcheck)
        ON_BN_CLICKED(IDC_GETIMAGE, OnGetimage)
        ON_BN_CLICKED(IDC_GETSTATE, OnGetstate)
        ON_BN_CLICKED(IDC_SETCHECK, OnSetcheck)
        ON_BN_CLICKED(IDC_SETIMAGE, OnSetimage)
        ON_BN_CLICKED(IDC_SETSTATE, OnSetstate)
        ON_BN_CLICKED(IDC_SETSTYLE, OnSetstyle)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonMessages message handlers


/****************************************************************************
*                        CButtonMessages::getCheckVal
* Result: int
*       
* Effect: 
*       Value of SetCheck: 0, 1, 2
****************************************************************************/

int CButtonMessages::getCheckVal()
    {
     if(c_2.GetCheck())
        return 2;
     else
     if(c_1.GetCheck())
        return 1;
     else
        return 0;
    }

/****************************************************************************
*                         CButtonMessages::getButton
* Result: CButton *
*       
* Effect: 
*       The button to send the message to
****************************************************************************/

CButton * CButtonMessages::getButton()
    {
     if(c_Button1Select.GetCheck())
        return c_Button1;

     if(c_Button2Select.GetCheck())
        return c_Button2;

     if(c_Button3Select.GetCheck())
        return c_Button3;

     return c_Button1;  // should never get here
    }


void CButtonMessages::OnButton1() 
{
 CString s;
 s.LoadString(IDS_BN_CLICKED);
 c_Messages->addMessage(m_hWnd, getButtonID(c_Button1), s);
}

void CButtonMessages::OnButton2() 
{
 CString s;
 s.LoadString(IDS_BN_CLICKED);
 c_Messages->addMessage(m_hWnd, getButtonID(c_Button2), s);
}

void CButtonMessages::OnButton3() 
{
 CString s;
 s.LoadString(IDS_BN_CLICKED);
 c_Messages->addMessage(m_hWnd, getButtonID(c_Button3), s);
}

void CButtonMessages::OnClick() 
{
 
 CButton * button = getButton();

 CString s;
 s.LoadString(IDS_BM_CLICK);
        
 c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);

 button->SendMessage(BM_CLICK);
}

void CButtonMessages::OnGetcheck() 
{
 CButton * button = getButton();
 int result = button->SendMessage(BM_GETCHECK); 

 CString t;
 t.LoadString(IDS_BM_GETCHECK);

 CString text;
 text.LoadString(IDS_BST_UNCHECKED + result);

 CString s;
 s.Format(_T("%s => %d (%s)"), t, result, text);
        
 c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);
}

void CButtonMessages::OnGetimage() 
{
 CButton * button = getButton();

 HICON oldicon = (HICON)button->SendMessage(BM_GETIMAGE);
     
 CString t;
 t.LoadString(IDS_BM_GETIMAGE);
     
 CString s;
 s.Format(_T("%s => 0x%08x"), t, oldicon);
 c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);
}

void CButtonMessages::OnGetstate() 
{
 CButton * button = getButton();
 int result = button->SendMessage(BM_GETSTATE); 
 
 CString t;
 t.LoadString(IDS_BM_GETSTATE);

 CString s;
 s.Format(_T("%s => 0x%04x"), t, result);
        
 c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);
        
}

void CButtonMessages::OnSetcheck() 
{
 CButton * button = getButton();
 
 int val = getCheckVal();

 button->SendMessage(BM_SETCHECK, val);

 CString t;
 t.LoadString(IDS_BM_SETCHECK);

 CString s;
 s.Format(_T("%s(%d, 0)"), t, val);

 c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);
        
}

void CButtonMessages::OnSetimage() 
{
 HICON icon;
 if(c_Button1Select.GetCheck())
    icon = icon1;
 else
 if(c_Button2Select.GetCheck())
    icon = icon2;
 else
    icon = icon3;

 setIcon(getButton(), icon);
        
}

void CButtonMessages::OnSetstate() 
{
 CButton * button = getButton();

 int val = c_StateVal.GetCheck();

 button->SendMessage(BM_SETSTATE, val);
        
 CString t;
 t.LoadString(IDS_BM_SETSTATE);

 CString s;
 s.Format(_T("%s(%d, 0)"), t, val);

 c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);

}

void CButtonMessages::OnSetstyle() 
{
        // TODO: Add your control notification handler code here
        
}


/****************************************************************************
*                          CButtonMessages::loadIcon
* Inputs:
*       int id: id of icon toload
* Result: HICON
*       Icon handle
* Effect: 
*       Loads the small icon whose id is given
****************************************************************************/

HICON CButtonMessages::loadIcon(int id)
    {
     return (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id),
                         IMAGE_ICON, 
                         ::GetSystemMetrics(SM_CXSMICON),
                         ::GetSystemMetrics(SM_CYSMICON),
                         LR_DEFAULTCOLOR);
     
    }

void CButtonMessages::setIcon(CButton * button, HICON icon)
    {

     HICON oldicon = (HICON)button->SendMessage(BM_SETIMAGE, 0, (LPARAM)icon);
     
     CString t;
     t.LoadString(IDS_BM_SETIMAGE);
     
     CString s;
     s.Format(_T("%s(0, 0x%08x) => 0x%08x"), t, icon, oldicon);
     c_Messages->addMessage(button->m_hWnd, getButtonID(button), s);
     
    }

BOOL CButtonMessages::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CRect r;
        GetWindowRect(&r);
        SetWindowPos(NULL,  
                        ::GetSystemMetrics(SM_CXSCREEN) - r.Width(), 0,
                        0, 0, SWP_NOSIZE | SWP_NOZORDER);
        
        CheckRadioButton(IDC_0, IDC_2, IDC_0);
        CheckRadioButton(IDC_BUTTON1, IDC_BUTTON3, IDC_BUTTON1);
        c_SetStyle.EnableWindow(FALSE);

        icon1 = loadIcon(IDI_ICON1);
        icon2 = loadIcon(IDI_ICON2);
        icon3 = loadIcon(IDI_ICON3);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
