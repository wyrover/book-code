// FormXPpg.cpp : Implementation of the CFormXPropPage property page class.

#include "stdafx.h"
#include "$$root$$.h"
#include "FormXPpg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFormXPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CFormXPropPage, COlePropertyPage)
    //{{AFX_MSG_MAP(CFormXPropPage)
    // NOTE - ClassWizard will add and remove message map entries
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CFormXPropPage, "FORMX.FormXPropPage.1",
                       0xafdc1588, 0xf993, 0x11d1, 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CFormXPropPage::CFormXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CFormXPropPage

BOOL CFormXPropPage::CFormXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
    if (bRegister)
        return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
                                               m_clsid, IDS_FORMX_PPG);
    else
        return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CFormXPropPage::CFormXPropPage - Constructor

CFormXPropPage::CFormXPropPage() :
    COlePropertyPage(IDD, IDS_FORMX_PPG_CAPTION)
{
    //{{AFX_DATA_INIT(CFormXPropPage)
    // NOTE: ClassWizard will add member initialization here
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CFormXPropPage::DoDataExchange - Moves data between page and properties

void CFormXPropPage::DoDataExchange(CDataExchange* pDX)
{
    //{{AFX_DATA_MAP(CFormXPropPage)
    // NOTE: ClassWizard will add DDP, DDX, and DDV calls here
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA_MAP
    DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CFormXPropPage message handlers
