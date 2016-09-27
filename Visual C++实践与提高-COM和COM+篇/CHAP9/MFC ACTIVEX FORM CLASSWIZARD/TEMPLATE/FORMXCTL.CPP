// FormXCtl.cpp : Implementation of the CFormXCtrl ActiveX Control class.

#include "stdafx.h"
#include "$$root$$.h"
#include "FormXCtl.h"
#include "FormXPpg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CFormXCtrl, CFormControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CFormXCtrl, CFormControl)
    //{{AFX_MSG_MAP(CFormXCtrl)
    //}}AFX_MSG_MAP
    ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CFormXCtrl, CFormControl)
    //{{AFX_DISPATCH_MAP(CFormXCtrl)
    // NOTE - ClassWizard will add and remove dispatch map entries
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CFormXCtrl, CFormControl)
    //{{AFX_EVENT_MAP(CFormXCtrl)
    // NOTE - ClassWizard will add and remove event map entries
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CFormXCtrl, 1)
    PROPPAGEID(CFormXPropPage::guid)
END_PROPPAGEIDS(CFormXCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CFormXCtrl, "FORMX.FormXCtrl.1",
                       0xafdc1587, 0xf993, 0x11d1, 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CFormXCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DFormX =
{ 0xafdc1585, 0xf993, 0x11d1, { 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0 } };
const IID BASED_CODE IID_DFormXEvents =
{ 0xafdc1586, 0xf993, 0x11d1, { 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwFormXOleMisc =
    OLEMISC_SIMPLEFRAME |
    OLEMISC_ACTIVATEWHENVISIBLE |
    OLEMISC_SETCLIENTSITEFIRST |
    OLEMISC_INSIDEOUT |
    OLEMISC_CANTLINKINSIDE |
    OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CFormXCtrl, IDS_FORMX, _dwFormXOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::CFormXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CFormXCtrl

BOOL CFormXCtrl::CFormXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
    // TODO: Verify that your control follows apartment-model threading rules.
    // Refer to MFC TechNote 64 for more information.
    // If your control does not conform to the apartment-model rules, then
    // you must modify the code below, changing the 6th parameter from
    // afxRegApartmentThreading to 0.
    if (bRegister)
        return AfxOleRegisterControlClass(
                   AfxGetInstanceHandle(),
                   m_clsid,
                   m_lpszProgID,
                   IDS_FORMX,
                   IDB_FORMX,
                   afxRegApartmentThreading,
                   _dwFormXOleMisc,
                   _tlid,
                   _wVerMajor,
                   _wVerMinor);
    else
        return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::CFormXCtrl - Constructor

CFormXCtrl::CFormXCtrl():
    CFormControl(CFormXCtrl::IDD, TRUE)
{
    InitializeIIDs(&IID_DFormX, &IID_DFormXEvents);
    EnableSimpleFrame();
    // TODO: Initialize your control's instance data here.
    //{{AFX_DATA_INIT(CFormXCtrl)
    //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::~CFormXCtrl - Destructor

CFormXCtrl::~CFormXCtrl()
{
    // TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::OnDraw - Drawing function

void CFormXCtrl::OnDraw(
    CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    if (!m_hWnd) {
        CFormControl::OnDraw(pdc, rcBounds, rcInvalid);
    } else {
        // TODO: Replace the following code with your own drawing code.
        pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
        pdc->Ellipse(rcBounds);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::DoPropExchange - Persistence support

void CFormXCtrl::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    CFormControl::DoPropExchange(pPX);
    // TODO: Call PX_ functions for each persistent custom property.
    // {{AFX_DATA(CFormXCtrl)
    enum {IDD = IDD_FORMX};
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl::OnResetState - Reset control to default state

void CFormXCtrl::OnResetState()
{
    CFormControl::OnResetState();  // Resets defaults found in DoPropExchange
    // TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CFormXCtrl message handlers

