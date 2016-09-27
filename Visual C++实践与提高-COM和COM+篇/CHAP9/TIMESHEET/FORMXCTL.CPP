// FormXCtl.cpp : Implementation of the CFormXCtrl ActiveX Control class.

#include "stdafx.h"
#include "timesheet.h"
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
    ON_EN_KILLFOCUS(IDC_TEXT_INPUT, OnKillfocusTextInput)
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



void CFormXCtrl::DoDataExchange(CDataExchange* pDX)
{
    //{{AFX_DATA_MAP(CXFormCtrl)
    DDX_Control(pDX, IDC_TEXT_INPUT, m_ctlTextInput);
    DDX_Control(pDX, IDC_MYMSFLEXGRID, m_ctlMSFlexGrid);
    //}}AFX_DATA_MAP
//  CFormControl::DoDataExchange(pDX);
}

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
    if (!m_hWnd) { //It's design time
        CFormControl::OnDraw(pdc, rcBounds, rcInvalid);
    } else { //It's runtime
        m_ctlMSFlexGrid.SetWindowPos(&wndTop,
                                     rcBounds.left,
                                     rcBounds.top,
                                     rcBounds.Width(),
                                     rcBounds.Height(),
                                     NULL);
        SetStaticHeadings();
    }

    /*  if (!m_hWnd)
        {
            CFormControl::OnDraw(pdc, rcBounds, rcInvalid);
        }
        else
        {

        // TODO: Replace the following code with your own drawing code.
            pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
            pdc->Ellipse(rcBounds);
        }*/
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

void CFormXCtrl::SetStaticHeadings()
{
    // Center all data
    m_ctlMSFlexGrid.SetColAlignment(-1, 4);
    // Set headings
    m_ctlMSFlexGrid.SetTextMatrix(2, 0, "項目");
    m_ctlMSFlexGrid.SetTextMatrix(2, 1, "任务号");
    m_ctlMSFlexGrid.SetTextMatrix(2, 2, "说明");
    m_ctlMSFlexGrid.SetTextMatrix(0, 3, "星期天");
    m_ctlMSFlexGrid.SetTextMatrix(0, 4, "星期一");
    m_ctlMSFlexGrid.SetTextMatrix(0, 5, "星期二");
    m_ctlMSFlexGrid.SetTextMatrix(0, 6, "星期三");
    m_ctlMSFlexGrid.SetTextMatrix(0, 7, "星期四");
    m_ctlMSFlexGrid.SetTextMatrix(0, 8, "星期五");
    m_ctlMSFlexGrid.SetTextMatrix(0, 9, "星期六");
    m_ctlMSFlexGrid.SetTextMatrix(1, 10, "总计");
    // Display the date heading
    DisplayDate();
    // Display hours heading
    CalculateHours();
}

void CFormXCtrl::DisplayDate()
{
    // Display whole week of date, starting Sunday.
    CTime tTime = CTime::GetCurrentTime(); // Get current day
    int i;
    CString sDate;
    int nDayOfWeek = tTime.GetDayOfWeek();
    tTime -= CTimeSpan(nDayOfWeek - 1, 0, 0, 0); // Get the Date for Sunday

    for (i = 0; i < 7; i++) {
        tTime += CTimeSpan(1, 0, 0, 0);
        sDate = tTime.Format("%x");
        m_ctlMSFlexGrid.SetTextMatrix(1, i + 3, sDate);
    }
}

void CFormXCtrl::CalculateHours()
{
    int i, j, k;
    double fDayTotalHours;
    double fWeekTotalHours = 0.0f;
    double fProjTotalHours;
    CString sHour;
    long lTotalRow = m_ctlMSFlexGrid.GetRows(); // total of rows

    //Comput each day total
    for (i = 3; i < 10; i++) { // Column 3 to 9
        fDayTotalHours = 0;

        for (j = 3; j < lTotalRow; j++) { // Row
            sHour = m_ctlMSFlexGrid.GetTextMatrix(j, i);
            fDayTotalHours += atof(sHour);
            sHour.Format("%2.1f", fDayTotalHours);
            m_ctlMSFlexGrid.SetTextMatrix(2, i, sHour);
            // Now compute each project hour
            fProjTotalHours = 0;

            for (k = 3; k < 10; k++) { // Column
                sHour = m_ctlMSFlexGrid.GetTextMatrix(j, k);
                fProjTotalHours += atof(sHour);
            }

            sHour.Format("%2.1f", fProjTotalHours);
            m_ctlMSFlexGrid.SetTextMatrix(j, 10, sHour);
        }

        fWeekTotalHours += fDayTotalHours;
    }

    sHour.Format("%2.1f", fWeekTotalHours);
    m_ctlMSFlexGrid.SetTextMatrix(2, 10, sHour);
}

BEGIN_EVENTSINK_MAP(CFormXCtrl, CFormControl)
//{{AFX_EVENTSINK_MAP(CFormXCtrl)
ON_EVENT(CFormXCtrl, IDC_MYMSFLEXGRID, -600 /* Click */, OnClickMymsflexgrid, VTS_NONE)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CFormXCtrl::OnClickMymsflexgrid()
{
    // Move IDC_TEXT_INPUT to the cell clicked
    long lRow = m_ctlMSFlexGrid.GetRowSel();
    long lCol = m_ctlMSFlexGrid.GetColSel();

    // if user clicks on column index 10, it's total hour, return it
    if (lCol == 10)
        return;

    // Get a DC
    CDC* pDC = GetDC(); // Get a device context
    ASSERT(pDC);
    // The MSFlexGrid function always return the twips, so we need to convert it to pixels
    // 1440 twips = 1 logical inch
    // Get dots(pixels) per logical inch, then 1440 twips is divided by it
    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    ReleaseDC(pDC);
    // Converting Twips to Dots (pixels)
    long lCellWidth = m_ctlMSFlexGrid.GetCellWidth() / nTwipsPerDotX;
    long lCellHeight = m_ctlMSFlexGrid.GetCellHeight() / nTwipsPerDotY;
    long lCellLeft = m_ctlMSFlexGrid.GetCellLeft() / nTwipsPerDotX;
    long lCellTop = m_ctlMSFlexGrid.GetCellTop() / nTwipsPerDotY;
    m_ctlTextInput.SetWindowPos(&wndTop,
                                lCellLeft,
                                lCellTop,
                                lCellWidth,
                                lCellHeight,
                                NULL);
    m_ctlTextInput.SetFocus();
    m_ctlTextInput.ShowWindow(SW_SHOW);
    // Clean CEdit Control string and read text back from current cell to the edit control
    SetDlgItemText(IDC_TEXT_INPUT, "");
    CString sData = m_ctlMSFlexGrid.GetText();
    m_ctlTextInput.ReplaceSel(sData);
}
void CFormXCtrl::OnKillfocusTextInput()
{
    // Copy text from CEdit control to current selected Grid cell
    // Get edit control data
    CString sInputData;
    GetDlgItemText(IDC_TEXT_INPUT, sInputData);

    // User did not input anything or erase content, return
    if (sInputData.IsEmpty()) {
        m_ctlMSFlexGrid.SetText("");
        return;
    }

    m_ctlMSFlexGrid.SetText(sInputData);
    // Now check if the user input data in one of column 3 to 9, check if it's number
    long lCol = m_ctlMSFlexGrid.GetColSel();

    if ((lCol > 2) && (lCol < 10)) {
        double fHour = atof(sInputData);

        if (fHour == 0.0f) { // User did not input number
            m_ctlMSFlexGrid.SetText("");
            AfxMessageBox("Please input number here!");
        }

        // Error checking working hour
        if (fHour > 24.0f) {
            m_ctlMSFlexGrid.SetText("");
            AfxMessageBox("Are you kidding? No one can work over 24 hours in one day!");
        }

        // Call calculate hour routine
        CalculateHours();
    }

    m_ctlTextInput.ShowWindow(SW_HIDE);
}