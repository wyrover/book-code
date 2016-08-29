// listops.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "listops.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CListOps::selection = LB_ERR;

/////////////////////////////////////////////////////////////////////////////
// CListOps property page

IMPLEMENT_DYNCREATE(CListOps, CLoggingPage)

CListOps::CListOps(int idd) : CLoggingPage(idd)
{
}

CListOps::CListOps() : CLoggingPage((int)0)  // should never call default constructor!
{
        //{{AFX_DATA_INIT(CListOps)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CListOps::~CListOps()
{
}

void CListOps::DoDataExchange(CDataExchange* pDX)
{
        CLoggingPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListOps)
        DDX_Control(pDX, IDC_SENDSINGLE, c_SendSingle);
        DDX_Control(pDX, IDC_SENDNONE, c_SendNone);
        DDX_Control(pDX, IDC_SENDMULTIPLE, c_SendMultiple);
        DDX_Control(pDX, IDC_SENDEXTENDED, c_SendExtended);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListOps, CLoggingPage)
        //{{AFX_MSG_MAP(CListOps)
        ON_BN_CLICKED(IDC_SENDSINGLE, OnSendSingleClicked)
        ON_BN_CLICKED(IDC_SENDMULTIPLE, OnSendMultipleClicked)
        ON_BN_CLICKED(IDC_SENDEXTENDED, OnSendExtendedClicked)
        ON_BN_CLICKED(IDC_SENDNONE, OnSendNoneClicked)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListOps message handlers

BOOL CListOps::OnSetActive()
{
 CWnd * focus = GetFocus();
 if(selection == LB_ERR)
    { /* use single */
     CheckRadioButton(IDC_SENDSINGLE, IDC_SENDNONE, IDC_SENDSINGLE);
     selection = IDC_SENDSINGLE;
    } /* use single */
 else
    CheckRadioButton(IDC_SENDSINGLE, IDC_SENDNONE, selection);

 BOOL result = CLoggingPage::OnSetActive();

 if(focus != NULL)
    focus->SetFocus();

 return result;
}

/****************************************************************************
*                       CListOps::OnSendSingleClicked
* Result: void
*       
* Effect: 
*       Sets the global selection to be the 'Single' listbox
****************************************************************************/

void CListOps::OnSendSingleClicked()
    {
     selection = IDC_SENDSINGLE;
    }

/****************************************************************************
*                       CListOps::OnSendMultipleClicked
* Result: void
*       
* Effect: 
*       Sets the global selection to be the 'Multiple' listbox
****************************************************************************/

void CListOps::OnSendMultipleClicked()
    {
     selection = IDC_SENDMULTIPLE;
    }

/****************************************************************************
*                       CListOps::OnSendExtendedClicked
* Result: void
*       
* Effect: 
*       Sets the global selection to be the 'Extended' listbox
****************************************************************************/

void CListOps::OnSendExtendedClicked()
    {
     selection = IDC_SENDEXTENDED;
    }

/****************************************************************************
*                       CListOps::OnSendNoneClicked
* Result: void
*       
* Effect: 
*       Sets the global selection to be the 'None' listbox
****************************************************************************/

void CListOps::OnSendNoneClicked()
    {
     selection = IDC_SENDNONE;
    }

/****************************************************************************
*                            CListOps::getListBox
* Result: CListBox *
*       Reference to one of the list boxes, based on the current selection
****************************************************************************/

CListBox * CListOps::getListBox()
    {
     if(c_SendSingle.GetCheck())
        return c_Single;
     if(c_SendExtended.GetCheck())
        return c_Extended;
     if(c_SendMultiple.GetCheck())
        return c_Multiple;
     if(c_SendNone.GetCheck())
        return c_None;
     return NULL; // should never get here
    }

/****************************************************************************
*                          CListOps::getListBoxName
* Result: CString
*       String name of list box
* Notes: 
*       This actually delivers back the caption of the button that was used
*       to select it; so these captions are important strings.
****************************************************************************/

CString CListOps::getListBoxName()
    {
     CString s;
     if(c_SendSingle.GetCheck())
        c_SendSingle.GetWindowText(s);
     else
     if(c_SendExtended.GetCheck())
        c_SendExtended.GetWindowText(s);
     else
     if(c_SendMultiple.GetCheck())
        c_SendMultiple.GetWindowText(s);
     else
     if(c_SendNone.GetCheck())
        c_SendNone.GetWindowText(s);

     int pos = s.Find(_T('&'));
     CString result;

     result = s.Left(pos) + s.Right(s.GetLength() - (pos + 1));
     return result;
    }

/****************************************************************************
*                            CListOps::addMessage
* Inputs:
*       CString s: Message to emit
* Result: void
*       
* Effect: 
*       Adds the message, given the list box
****************************************************************************/

void CListOps::addMessage(CString s)
    {
     CListBox * box = getListBox();

     c_Messages->addMessage(box->m_hWnd, getListBoxName(), s); // TODO: fix up name
    }
