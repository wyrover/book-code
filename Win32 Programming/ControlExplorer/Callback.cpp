// Callback.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "NumericEdit.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "treeOps.h"
#include "tvitem.h"
#include "Callback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CCallback::UWM_RECORD_DISPINFO = 0;

/////////////////////////////////////////////////////////////////////////////
// CCallback dialog

IMPLEMENT_DYNCREATE(CCallback, CTV_ITEM)

CCallback::CCallback() : CTV_ITEM(CCallback::IDD)
{
        UWM_RECORD_DISPINFO = ::RegisterWindowMessage(_T("Record.TVN_DISPINFO"));
        //{{AFX_DATA_INIT(CCallback)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CCallback::~CCallback()
{
}

void CCallback::DoDataExchange(CDataExchange* pDX)
{
        CTV_ITEM::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CCallback)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallback, CTV_ITEM)
        //{{AFX_MSG_MAP(CCallback)
        ON_WM_CLOSE()
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallback message handlers

BOOL CCallback::OnInitDialog() 
{
        CTV_ITEM::OnInitDialog();
        
        // We don't allow the callback option for anything at this point

        for(int i = 0; i < c_cChildren.GetCount(); i++)
           { /* delete callback option */
            if(c_cChildren.GetItemData(i) == I_CHILDRENCALLBACK)
               { /* found it */
                c_cChildren.DeleteString(i);
                break;
               } /* found it */
           } /* delete callback option */

        itemToControls(item);  // get item values
        
        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallback::OnClose() 
{
        CTV_ITEM::OnClose();
}

/****************************************************************************
*                          CCallback::enableControls
* Result: void
*       
* Effect: 
*       Enables the controls for this item
****************************************************************************/

void CCallback::enableControls()
    {
     CTV_ITEM::enableControls();
     c_iImage_Callback.EnableWindow(FALSE);
     c_iSelectedImage_Callback.EnableWindow(FALSE);
     c_pszText_Callback.EnableWindow(FALSE);
     c_TVIF_STATE.EnableWindow(FALSE);
     c_TVIF_CHILDREN.EnableWindow(FALSE);
     c_TVIF_HANDLE.EnableWindow(FALSE);
     c_TVIF_IMAGE.EnableWindow(FALSE);
     c_TVIF_SELECTEDIMAGE.EnableWindow(FALSE);
     c_TVIF_TEXT.EnableWindow(FALSE);
     c_TVIF_PARAM.EnableWindow(FALSE);
    }

void CCallback::OnDestroy() 
{
        CTV_ITEM::OnDestroy();
        
        controlsToItem(item, TRUE);
        controller->SendMessage(UWM_RECORD_DISPINFO, 0, (LPARAM)item);
}
