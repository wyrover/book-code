// CircPpg.cpp : Implementation of the CCircPropPage property page class.

#include "stdafx.h"
#include "MfcCircCtrl.h"
#include "CircPpg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCircPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCircPropPage, COlePropertyPage)
    //{{AFX_MSG_MAP(CCircPropPage)
    // NOTE - ClassWizard will add and remove message map entries
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCircPropPage, "MFCCIRCCTRL.CircPropPage.1",
                       0x7f13145e, 0x1d96, 0x4654, 0xb9, 0xdd, 0x98, 0x5b, 0xc, 0x7b, 0x32, 0x7c)


/////////////////////////////////////////////////////////////////////////////
// CCircPropPage::CCircPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCircPropPage

BOOL CCircPropPage::CCircPropPageFactory::UpdateRegistry(BOOL bRegister)
{
    if (bRegister)
        return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
                                               m_clsid, IDS_CIRC_PPG);
    else
        return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCircPropPage::CCircPropPage - Constructor

CCircPropPage::CCircPropPage() :
    COlePropertyPage(IDD, IDS_CIRC_PPG_CAPTION)
{
    //{{AFX_DATA_INIT(CCircPropPage)
    m_caption = _T("");
    m_circleOffset = 0;
    m_circleShape = FALSE;
    //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCircPropPage::DoDataExchange - Moves data between page and properties

void CCircPropPage::DoDataExchange(CDataExchange* pDX)
{
    //{{AFX_DATA_MAP(CCircPropPage)
    DDP_Text(pDX, IDC_CAPTION, m_caption, _T("Caption"));
    DDX_Text(pDX, IDC_CAPTION, m_caption);
    DDP_Text(pDX, IDC_CIRCLEOFFSET, m_circleOffset, _T("CircleOffset"));
    DDX_Text(pDX, IDC_CIRCLEOFFSET, m_circleOffset);
    DDP_Check(pDX, IDC_CIRCLESHAPE, m_circleShape, _T("CircleShape"));
    DDX_Check(pDX, IDC_CIRCLESHAPE, m_circleShape);
    //}}AFX_DATA_MAP
    DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCircPropPage message handlers
