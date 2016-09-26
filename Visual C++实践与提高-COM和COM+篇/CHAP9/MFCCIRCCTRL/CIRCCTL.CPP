// CircCtl.cpp : Implementation of the CCircCtrl ActiveX Control class.

#include "stdafx.h"
#include "MfcCircCtrl.h"
#include "CircCtl.h"
#include "CircPpg.h"


#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCircCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCircCtrl, COleControl)
    //{{AFX_MSG_MAP(CCircCtrl)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
    ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCircCtrl, COleControl)
    //{{AFX_DISPATCH_MAP(CCircCtrl)
    DISP_PROPERTY_NOTIFY(CCircCtrl, "CircleShape", m_circleShape, OnCircleShapeChanged, VT_BOOL)
    DISP_PROPERTY_EX(CCircCtrl, "CircleOffset", GetCircleOffset, SetCircleOffset, VT_I2)
    DISP_STOCKPROP_BACKCOLOR()
    DISP_STOCKPROP_CAPTION()
    DISP_STOCKPROP_FONT()
    DISP_STOCKPROP_FORECOLOR()
    //}}AFX_DISPATCH_MAP
    DISP_FUNCTION_ID(CCircCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCircCtrl, COleControl)
    //{{AFX_EVENT_MAP(CCircCtrl)
    EVENT_CUSTOM("ClickIn", FireClickIn, VTS_XPOS_PIXELS  VTS_YPOS_PIXELS)
    EVENT_CUSTOM("ClickOut", FireClickOut, VTS_NONE)
    //}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCircCtrl, 3)
    PROPPAGEID(CCircPropPage::guid)
    PROPPAGEID(CLSID_CColorPropPage)
    PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CCircCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCircCtrl, "MFCCIRCCTRL.CircCtrl.1",
                       0x6ce7be10, 0x45d4, 0x4f76, 0xbc, 0x92, 0x84, 0x2f, 0xfc, 0x77, 0xec, 0xb3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCircCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCirc =
{ 0x687e1bb1, 0xdfd0, 0x4a2e, { 0x97, 0x86, 0xc2, 0xcf, 0x6f, 0xd6, 0xfa, 0x46 } };
const IID BASED_CODE IID_DCircEvents =
{ 0xfd2eb05, 0xc43d, 0x4a7d, { 0xb6, 0x4e, 0xad, 0x28, 0x6f, 0x8b, 0x92, 0x67 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCircOleMisc =
    OLEMISC_ACTIVATEWHENVISIBLE |
    OLEMISC_SETCLIENTSITEFIRST |
    OLEMISC_INSIDEOUT |
    OLEMISC_CANTLINKINSIDE |
    OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCircCtrl, IDS_CIRC, _dwCircOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::CCircCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCircCtrl

BOOL CCircCtrl::CCircCtrlFactory::UpdateRegistry(BOOL bRegister)
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
                   IDS_CIRC,
                   IDB_CIRC,
                   afxRegApartmentThreading,
                   _dwCircOleMisc,
                   _tlid,
                   _wVerMajor,
                   _wVerMinor);
    else
        return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::CCircCtrl - Constructor

CCircCtrl::CCircCtrl()
{
    InitializeIIDs(&IID_DCirc, &IID_DCircEvents);
    // TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::~CCircCtrl - Destructor

CCircCtrl::~CCircCtrl()
{
    // TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::OnDraw - Drawing function

void CCircCtrl::OnDraw(
    CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    CBrush* pOldBrush;
    CBrush bkBrush(TranslateColor(GetBackColor()));
    CPen* pOldPen;
    CRect rc = rcBounds;
    CFont* pOldFont;
    TEXTMETRIC tm;
    const CString& strCaption = InternalGetText();
    // Set the ForeColor property color and transparent background mode into the device context
    pdc->SetTextColor(TranslateColor(GetForeColor()));
    pdc->SetBkMode(TRANSPARENT);
    // Paint the background using the BackColor property
    pdc->FillRect(rcBounds, &bkBrush);
    // Draw the ellipse using the BackColor property and a black pen
    GetDrawRect(&rc);
    pOldBrush = pdc->SelectObject(&bkBrush);
    pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);
    pdc->Ellipse(rc);
    // Draw the caption and note using the stock Font and ForeColor properties
    pOldFont = SelectStockFont(pdc);
    GetStockTextMetrics(&tm);
    pdc->SetTextAlign(TA_CENTER | TA_TOP);
    pdc->ExtTextOut((rc.left + rc.right) / 2, (rc.top + rc.bottom - tm.tmHeight) / 2,
                    ETO_CLIPPED, rc, strCaption, strCaption.GetLength(), NULL);
//  pdc->SetTextAlign(TA_LEFT | TA_TOP);
//  pdc->ExtTextOut(rcBounds.left, rcBounds.top,
//      ETO_CLIPPED, rcBounds, m_note, m_note.GetLength(), NULL);
    pdc->SelectObject(pOldFont);
    pdc->SelectObject(pOldPen);
    pdc->SelectObject(pOldBrush);
}



/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::DoPropExchange - Persistence support

void CCircCtrl::DoPropExchange(CPropExchange* pPX)
{
    ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
    COleControl::DoPropExchange(pPX);
    // TODO: Call PX_ functions for each persistent custom property.
    PX_Bool(pPX, "CircleShape", m_circleShape, TRUE);
    PX_Short(pPX, _T("CircleOffset"), m_circleOffset, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::OnResetState - Reset control to default state

void CCircCtrl::OnResetState()
{
    COleControl::OnResetState();  // Resets defaults found in DoPropExchange
    // TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl::AboutBox - Display an "About" box to the user

void CCircCtrl::AboutBox()
{
    CDialog dlgAbout(IDD_ABOUTBOX_CIRC);
    dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCircCtrl message handlers

void CCircCtrl::OnCircleShapeChanged()
{
    // force the control to redraw itself
    InvalidateControl();
    SetModifiedFlag();

    // reset the circle offset, if necessary
    if (m_circleShape)
        SetCircleOffset(0);
}

short CCircCtrl::GetCircleOffset()
{
    return m_circleOffset;
}

void CCircCtrl::SetCircleOffset(short nNewValue)
{
    // Validate the specified offset value
    if ((m_circleOffset != nNewValue) && m_circleShape && InBounds(nNewValue)) {
        m_circleOffset = nNewValue;
        SetModifiedFlag();
        InvalidateControl();
    }
}


void CCircCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (InCircle(point))
        // Fire the ClickIn event
        FireClickIn(point.x, point.y);
    else
        // Fire the ClickOut event
        FireClickOut();

    COleControl::OnLButtonDown(nFlags, point);
}



BOOL CCircCtrl::InBounds(short nOffset)
{
    CRect rc;
    int diameter;
    int length;
    int cx, cy;
    GetControlSize(&cx, &cy);

    if (cx > cy) {
        length = cx;
        diameter = cy;
    } else {
        length = cy;
        diameter = cx;
    }

    if (nOffset < 0)
        nOffset = -nOffset;

    return (diameter / 2 + nOffset) <= (length / 2);
}

BOOL CCircCtrl::InCircle(CPoint &point)
{
    CRect rc;
    GetClientRect(rc);
    GetDrawRect(&rc);
    // Determine radii
    double a = (rc.right - rc.left) / 2;
    double b = (rc.bottom - rc.top) / 2;
    // Determine x, y
    double x = point.x - (rc.left + rc.right) / 2;
    double y = point.y - (rc.top + rc.bottom) / 2;
    // Apply ellipse formula
    return ((x * x) / (a * a) + (y * y) / (b * b) <= 1);
}

void CCircCtrl::GetDrawRect(CRect *rc)
{
    if (m_circleShape) {
        int cx = rc->right - rc->left;
        int cy = rc->bottom - rc->top;

        if (cx > cy) {
            rc->left += (cx - cy) / 2;
            rc->right = rc->left + cy;
            // offset circle in bounding rect
            rc->left += m_circleOffset;
            rc->right += m_circleOffset;
        } else {
            rc->top += (cy - cx) / 2;
            rc->bottom = rc->top + cx;
            // offset circle in bounding rect
            rc->bottom -= m_circleOffset;
            rc->top -= m_circleOffset;
        }
    }
}
