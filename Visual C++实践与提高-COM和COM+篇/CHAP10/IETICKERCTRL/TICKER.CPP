// Ticker.cpp : Implementation of CTicker

#include "stdafx.h"
#include "AtlTickerCtrl.h"
#include "Ticker.h"
//default font
FONTDESC DefaultFont = {
    sizeof(FONTDESC),
    OLESTR("Arial"),
    FONTSIZE(10),
    FW_BOLD,         // weight
    DEFAULT_CHARSET, // char set
    FALSE,           // italic
    FALSE,           // underline
    FALSE            // strike-through
};
/////////////////////////////////////////////////////////////////////////////
// CTicker
CTicker * g_pCtrl;
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    g_pCtrl->Scroll();
}

STDMETHODIMP CTicker::StopScroll()
{
    //Kill the timer if it exists
    if (m_Timer)
        ::KillTimer(NULL, m_Timer);

    m_Timer = NULL;
    //reset the scrolling mode
    m_bScrollingMode = FALSE;
    return S_OK;
}

STDMETHODIMP CTicker::StartScroll()
{
    //stop scrolling
    StopScroll();
    //set the scrolling mode
    m_bScrollingMode = TRUE;
    //create the timer for  scrolling
    g_pCtrl = this;
    m_Timer =::SetTimer(NULL, 1, m_speed, TimerProc);
    return S_OK;
}

void CTicker::Scroll()
{
    m_textStart = m_textStart - m_step;
    m_textEnd = m_textEnd - m_step;
    CComControlBase::FireViewChange();
}

STDMETHODIMP CTicker::get_Speed(long *pVal)
{
    *pVal = m_speed;
    return S_OK;
}

STDMETHODIMP CTicker::put_Speed(long newVal)
{
    //make sure we get a valid value
    if (newVal < 0)
        return E_INVALIDARG;

    m_speed = newVal;

    //re-create timer if scrolling is on
    if (m_bScrollingMode)
        StartScroll();

    FireOnChanged(0x4);
    return S_OK;
}

STDMETHODIMP CTicker::GetQuotes(VARIANT_BOOL *pbSuccess)
{
    //Set the stock quotes to a hardcoded string
    m_sText = "水清木华 575 +131.00, 广州超讯 493 +121.23, 凯信科技 145.33 +80.04";
    //return TRUE
    *pbSuccess = VARIANT_TRUE;
    //tell the container we want to redraw
    CComControlBase::FireViewChange();
    //Fire the GotQuotes event
    Fire_GotQuotes(m_sText.m_str);
    return S_OK;
}

HRESULT CTicker::OnDraw(ATL_DRAWINFO& di)
{
    USES_CONVERSION;
    //Get the drawing rectangle
    RECT& rect = *(LPRECT)di.prcBounds;
    //The device context
    HDC hdc = di.hdcDraw;
    //Brush used for drawing border
    HBRUSH hBrush;
    //colors used with GDI APIs
    COLORREF backClr;
    COLORREF foreClr;
    COLORREF borderClr;
    //translate from OLE colors to COLORREFs
    ::OleTranslateColor(m_clrBackColor, NULL, &backClr);
    ::OleTranslateColor(m_clrBorderColor, NULL, &borderClr);
    ::OleTranslateColor(m_clrForeColor, NULL, &foreClr);
    //create the bruch for the rectangle
    hBrush = CreateSolidBrush(backClr);
    FillRect(hdc, &rect, hBrush);
    //create the brush for the border
    hBrush = CreateSolidBrush(borderClr);
    FrameRect(hdc, &rect, hBrush);
    //Set the text font
    IFont *pIFont;
    //Query the m_pFont for an IFont interface
    m_pFont->QueryInterface(IID_IFont, reinterpret_cast<void**>(&pIFont));
    HFONT curFont, OrigFont;
    //get windows font handle
    pIFont->get_hFont(&curFont);
    //make sure the font stays valid until we are done
    pIFont->AddRefHfont(curFont);
    //select the font into the device
    //and keep track of the original font
    OrigFont = (HFONT)SelectObject(hdc, curFont);
    //set text backcolor and forecolor
    SetBkColor(hdc, backClr);
    SetTextColor(hdc, foreClr);

    //see if we are supposed to be running
    if (m_bScrollingMode) {
        //reposition the text if it
        //has scrolled off the control
        if (m_textEnd <= rect.left)
            FirstPosition(di);

        //draw the text on the rectangle
        SetTextAlign(hdc, TA_LEFT | VTA_BASELINE);
        ExtTextOut(hdc, m_textStart, rect.top + (rect.bottom - rect.top) / 2,
                   ETO_CLIPPED, &rect, W2A(m_sText.m_str),
                   lstrlen(W2A(m_sText.m_str)), NULL);
    } else {
        //we're not in run-mode
        //draw a fixed caption
        LPSTR szCaption = "AtlTickerCtrl Stock Ticker";
        SetTextAlign(hdc, TA_CENTER | VTA_BASELINE);
        ExtTextOut(hdc, rect.left + (rect.right - rect.left) / 2,
                   rect.top + (rect.bottom - rect.top) / 2, ETO_CLIPPED, &rect, szCaption,
                   lstrlen(szCaption), NULL);
    }

    SelectObject(hdc, OrigFont);
    //release the lock on the font
    pIFont->ReleaseHfont(curFont);
    //release the interface reference
    pIFont->Release();
    return S_OK;
}

void CTicker::FirstPosition(ATL_DRAWINFO &di)
{
    USES_CONVERSION;
    //get the drawing rectangle
    RECT& rect = *(LPRECT)di.prcBounds;
    //set text start to far right
    m_textStart = rect.right;
    //get the length of the text
    SIZE size;
    GetTextExtentPoint32(di.hdcDraw, W2A(m_sText.m_str), lstrlen(W2A(m_sText.m_str)), &size);
    //set the end of the text to start + length
    m_textEnd = m_textStart + size.cx;
}
STDMETHODIMP CTicker::OnAmbientPropertyChange(DISPID dispid)
{
    //check if the changed property is UserMode
    //or if there are multiple changed properties
    if (DISPID_AMBIENT_USERMODE == dispid || DISPID_UNKNOWN == dispid) {
        if (IsInUserMode())
            StartScroll();
        else
            StopScroll();
    }

    return S_OK;
}

BOOL CTicker::IsInUserMode()
{
    BOOL bUserMode = FALSE;
    //call CComControl::GetAmbientUserMode
    HRESULT hr = GetAmbientUserMode(bUserMode);

    //Not all control sites implement this property
    if (FAILED(hr))
        bUserMode = FALSE;

    return bUserMode;
}
STDMETHODIMP CTicker::SetClientSite(LPOLECLIENTSITE pSite)
{
    //call default implementation
    HRESULT hr = CComControlBase::IOleObject_SetClientSite(pSite);

    if (!pSite)
        return hr;

    //set default colors
    if (FAILED(GetAmbientBackColor(m_clrBackColor)))
        m_clrBackColor = (OLE_COLOR)RGB(255, 255, 255);

    if (FAILED(GetAmbientForeColor(m_clrForeColor)))
        m_clrForeColor = (OLE_COLOR)RGB(0, 0, 0);

    m_clrBorderColor = m_clrBackColor;

    //set default font
    if (m_pFont)
        m_pFont.Release();

    if (FAILED(GetAmbientFontDisp(static_cast<IFontDisp**>(&m_pFont)))) {
        OleCreateFontIndirect(&DefaultFont,
                              IID_IFontDisp,
                              reinterpret_cast<void**>(&m_pFont));
    }

    //get quotes & start scrolling if in user mode
    if (IsInUserMode()) {
        VARIANT_BOOL bSuccess;
        GetQuotes(&bSuccess);

        //scroll if we were successful
        if (VARIANT_TRUE == bSuccess)
            StartScroll();
    }

    return hr;
}
