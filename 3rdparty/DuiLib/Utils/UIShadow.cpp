#include "StdAfx.h"
#include "math.h"
#include "crtdbg.h"

// Some extra work to make this work in VC++ 6.0

// walk around the for iterator scope bug of VC++6.0
#ifdef _MSC_VER
#if _MSC_VER == 1200
#define for if(false);else for
#endif
#endif

// Some definitions for VC++ 6.0 without newest SDK
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA 0x01
#endif

#ifndef ULW_ALPHA
#define ULW_ALPHA 0x00000002
#endif

// Vista aero related message
#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED 0x031E
#endif

namespace DuiLib
{

const TCHAR *strWndClassName = _T("DGShadowUI");
bool CShadowUI::s_bHasInit = false;
bool CShadowUI::s_bVista = false;
CShadowUI::pfnDwmIsCompositionEnabled CShadowUI::s_DwmIsCompositionEnabled = NULL;
CShadowUI::pfnUpdateLayeredWindow CShadowUI::s_UpdateLayeredWindow = NULL;

CShadowUI::CShadowUI(void)
: m_hWnd((HWND)NULL)
, m_nDarkness(150)
, m_nSharpness(15)
, m_nSize(10)
, m_nxOffset(0)
, m_nyOffset(0)
, m_Color(RGB(50,50,50))
, m_WndSize(0)
, m_bUpdate(false)
, m_bIsImageMode(false)
, m_bIsShowShadow(false)
, m_Status(SS_ENABLED)
{
    ::ZeroMemory(&m_rcShadowCorner, sizeof(RECT));
}

CShadowUI::~CShadowUI(void)
{
}

bool CShadowUI::Initialize(HINSTANCE hInstance)
{
    // Should not initiate more than once
    if (NULL != s_UpdateLayeredWindow)
        return false;

    HMODULE hSysDll = LoadLibrary(_T("USER32.DLL"));
    s_UpdateLayeredWindow = 
        (pfnUpdateLayeredWindow)GetProcAddress(hSysDll, 
        "UpdateLayeredWindow");

    // If the import did not succeed, probably layered window is not supported by current OS
    if (NULL == s_UpdateLayeredWindow)
        return false;

    hSysDll = LoadLibrary(_T("dwmapi.dll"));
    if(hSysDll)    // Loaded dwmapi.dll succefull, must on Vista or above
    {
        s_bVista = true;
        s_DwmIsCompositionEnabled = 
            (pfnDwmIsCompositionEnabled)GetProcAddress(hSysDll, 
            "DwmIsCompositionEnabled");
    }

    // Register window class for shadow window
    WNDCLASSEX wcex;

    memset(&wcex, 0, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX); 
    wcex.style            = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = DefWindowProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance        = hInstance;
    wcex.hIcon            = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName    = NULL;
    wcex.lpszClassName    = strWndClassName;
    wcex.hIconSm        = NULL;

    RegisterClassEx(&wcex);

    s_bHasInit = true;
    return true;
}

void CShadowUI::Create(HWND hWndParent, CPaintManagerUI* pPaintManager)
{
    if(NULL == s_UpdateLayeredWindow)
        return;

    // Already initialized
    _ASSERT(CPaintManagerUI::GetInstance() != INVALID_HANDLE_VALUE);
    _ASSERT(pPaintManager != NULL);
    m_pManager = pPaintManager;
    //HWND hParentWnd = m_pManager->GetPaintWindow();

    m_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, strWndClassName, NULL,
        /*WS_VISIBLE | styleValue |*/ WS_POPUPWINDOW,
        CW_USEDEFAULT, 0, 0, 0, hWndParent, NULL, CPaintManagerUI::GetInstance(), NULL);

    // Determine the initial show state of shadow according to Aero
    m_Status = SS_ENABLED;    // Enabled by default
    BOOL bAero = FALSE;
    if(s_DwmIsCompositionEnabled)
        s_DwmIsCompositionEnabled(&bAero);
    //if (bAero)    m_Status |= SS_DISABLEDBYAERO;

    Show(hWndParent);    // Show the shadow if conditions are met

/*
    // Replace the original WndProc of parent window to steal messages
    m_OriParentProc = GetWindowLongPtr(hParentWnd, GWLP_WNDPROC);

#pragma warning(disable: 4311)    // temporrarily disable the type_cast warning in Win32
    SetWindowLongPtr(hParentWnd, GWLP_WNDPROC, (LONG)ParentProc);
#pragma warning(default: 4311)*/

}

LRESULT CALLBACK CShadowUI::ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_MOVE:
        if(this->m_Status & SS_VISABLE)
        {
            RECT WndRect;
            GetWindowRect(hwnd, &WndRect);
            int x = 0, y = 0; 
            if (m_bIsImageMode){
                if(m_sShadowImage.empty())
                    return 0;

                x = WndRect.left - m_rcShadowCorner.left;
                y = WndRect.top - m_rcShadowCorner.top;
            } else {
                x = WndRect.left + this->m_nxOffset - this->m_nSize;
                y = WndRect.top + this->m_nyOffset - this->m_nSize;
            }
            
            SetWindowPos(this->m_hWnd, 0, x, y,
                0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
        }
        break;

    case WM_SIZE:
        if(this->m_Status & SS_ENABLED && !(this->m_Status & SS_DISABLEDBYAERO))
        {
            if(SIZE_MAXIMIZED == wParam || SIZE_MINIMIZED == wParam)
            {
                ShowWindow(this->m_hWnd, SW_HIDE);
                this->m_Status &= ~SS_VISABLE;
            }
            else
            {
                LONG lParentStyle = GetWindowLong(hwnd, GWL_STYLE);
                if(WS_VISIBLE & lParentStyle)    // Parent may be resized even if invisible
                {
                    this->m_Status |= SS_PARENTVISIBLE;
                    if(!(this->m_Status & SS_VISABLE))
                    {
                        this->m_Status |= SS_VISABLE;
                        // Update before show, because if not, restore from maximized will
                        // see a glance misplaced shadow
                        this->Update(hwnd);
                        ShowWindow(this->m_hWnd, SW_SHOWNA);
                        // If restore from minimized, the window region will not be updated until WM_PAINT:(
                        this->m_bUpdate = true;
                    }
                    // Awful! It seems that if the window size was not decreased
                    // the window region would never be updated until WM_PAINT was sent.
                    // So do not Update() until next WM_PAINT is received in this case
                    else if(LOWORD(lParam) > LOWORD(this->m_WndSize) || HIWORD(lParam) > HIWORD(this->m_WndSize))
                        this->m_bUpdate = true;
                    else
                        this->Update(hwnd);
                }

            }
            this->m_WndSize = lParam;
        }
        break;

    case WM_PAINT:
        {
            if(this->m_bUpdate)
            {
                this->Update(hwnd);
                this->m_bUpdate = false;
            }
            //return hr;
            break;
        }

        // In some cases of sizing, the up-right corner of the parent window region would not be properly updated
        // Update() again when sizing is finished
    case WM_EXITSIZEMOVE:
        if(this->m_Status & SS_VISABLE)
        {
            this->Update(hwnd);
        }
        break;

/*
    case WM_SHOWWINDOW:
        if(this->m_Status & SS_ENABLED && !(this->m_Status & SS_DISABLEDBYAERO))
        {
            LRESULT lResult =  pDefProc(hwnd, uMsg, wParam, lParam);
            if((int)wParam == SW_HIDE)    // the window is being hidden
            {
                ShowWindow(this->m_hWnd, SW_HIDE);
                this->m_Status &= ~(SS_VISABLE | SS_PARENTVISIBLE);
            }
            else
            {
//                 this->m_Status |= SS_VISABLE | SS_PARENTVISIBLE;
//                 ShowWindow(this->m_hWnd, SW_SHOWNA);
//                 this->Update(hwnd);
                this->m_bUpdate = true;
                this->Show(hwnd);
            }
            return lResult;
        }
        break;
*/
    case WM_SHOWWINDOW:
        {
            if(this->m_Status & SS_ENABLED/* && !(this->m_Status & SS_DISABLEDBYAERO)*/)
            {
                if(!wParam)    // the window is being hidden
                {
                    ShowWindow(this->m_hWnd, SW_HIDE);
                    this->m_Status &= ~(SS_VISABLE | SS_PARENTVISIBLE);
                }
                else
                {
                    this->m_bUpdate = true;
                    this->Show(hwnd);
                    LONG lParentStyle = GetWindowLong(hwnd, GWL_STYLE);
                    if (lParentStyle & 0x80000000)
                    {
                        CDuiRect rect;
                        GetClientRect(hwnd,&rect);
                        PostMessage(hwnd,WM_SIZE,rect.GetWidth(),rect.GetHeight());
                    }
                    // this->m_Status |= SS_VISABLE | SS_PARENTVISIBLE;
                    // ShowWindow(this->m_hWnd, SW_SHOWNA);
                    // this->Update(hwnd);
                    //this->m_bUpdate = true;
                    //this->Show(hwnd);
                }
                return 0;
            }
            break;

        }
    case WM_DESTROY:
        DestroyWindow(this->m_hWnd);    // Destroy the shadow
        break;
    case WM_DWMCOMPOSITIONCHANGED:
        {
            BOOL bAero = FALSE;
            if(s_DwmIsCompositionEnabled)    // "if" is actually not necessary here :P
                s_DwmIsCompositionEnabled(&bAero);
            if (bAero)
                this->m_Status |= SS_DISABLEDBYAERO;
            else
                this->m_Status &= ~SS_DISABLEDBYAERO;
            
            this->Show(hwnd);
        }
        break;

    }

    return 0;
}

void CShadowUI::Update(HWND hParent)
{
    RECT WndRect;
    GetWindowRect(hParent, &WndRect);

    if (WndRect.left == 0 && WndRect.top == 0){
        return;
    }

    int nShadWndWid;
    int nShadWndHei;
    if (m_bIsImageMode){
        if(m_sShadowImage.empty())
            return;

        nShadWndWid = WndRect.right - WndRect.left + m_rcShadowCorner.left + m_rcShadowCorner.right;
        nShadWndHei = WndRect.bottom - WndRect.top + m_rcShadowCorner.top + m_rcShadowCorner.bottom;
    } else {
        nShadWndWid = WndRect.right - WndRect.left + m_nSize * 2;
        nShadWndHei = WndRect.bottom - WndRect.top + m_nSize * 2;
    }
        
    // Create the alpha blending bitmap
    BITMAPINFO bmi;        // bitmap header

    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = nShadWndWid;
    bmi.bmiHeader.biHeight = nShadWndHei;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = nShadWndWid * nShadWndHei * 4;

    BYTE *pvBits;          // pointer to DIB section
    HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void **)&pvBits, NULL, 0);
    HDC hMemDC = CreateCompatibleDC(NULL);
    HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hbitmap);

    if (m_bIsImageMode){
        RECT rcPaint = {0, 0, nShadWndWid, nShadWndHei};
        const TImageInfo* data = m_pManager->GetImageEx(m_sShadowImage.c_str(), NULL, 0);

        if( !data ) 
            return;

        RECT rcBmpPart = {0};
        rcBmpPart.right = data->nX;
        rcBmpPart.bottom = data->nY;

        CRenderEngine::DrawImage(hMemDC, data->hBitmap, rcPaint, rcPaint, rcBmpPart, 
            m_rcShadowCorner, data->alphaChannel, 0xFF, 0, true, false, false);
    } else {
        ZeroMemory(pvBits, bmi.bmiHeader.biSizeImage);
        MakeShadow((UINT32 *)pvBits, hParent, &WndRect);
    }

    POINT ptDst;
    if (m_bIsImageMode) {
        ptDst.x = WndRect.left - m_rcShadowCorner.left;
        ptDst.y = WndRect.top - m_rcShadowCorner.top;
    } else {
        ptDst.x = WndRect.left + m_nxOffset - m_nSize;
        ptDst.y = WndRect.top + m_nyOffset - m_nSize;
    }

    POINT ptSrc = {0, 0};
    SIZE WndSize = {nShadWndWid, nShadWndHei};
    BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    MoveWindow(m_hWnd, ptDst.x, ptDst.y, nShadWndWid, nShadWndHei, FALSE);

    BOOL bRet= ::UpdateLayeredWindow(m_hWnd, NULL, &ptDst, &WndSize, hMemDC,
        &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

    _ASSERT(bRet); // something was wrong....

    // Delete used resources
    SelectObject(hMemDC, hOriBmp);
    DeleteObject(hbitmap);
    DeleteDC(hMemDC);

}

void CShadowUI::Show(HWND hParentWnd)
{
    // Clear all except the enabled status
    m_Status &= SS_ENABLED | SS_DISABLEDBYAERO;
    //SetStatus(m_Status & (SS_ENABLED | SS_DISABLEDBYAERO));
    if((m_Status & SS_ENABLED) && !(m_Status & SS_DISABLEDBYAERO))    // Enabled
    {
        // Determine the show state of shadow according to parent window's state
        LONG lParentStyle = GetWindowLong(hParentWnd, GWL_STYLE);

        if(WS_VISIBLE & lParentStyle)    // Parent visible
        {
            m_Status |= SS_PARENTVISIBLE;

            // Parent is normal, show the shadow
            if(!((WS_MAXIMIZE | WS_MINIMIZE) & lParentStyle))    // Parent visible but does not need shadow
                m_Status |= SS_VISABLE;
        }
    }

    if(m_Status & SS_VISABLE)
    {
        ShowWindow(m_hWnd, SW_SHOWNA);
        Update(hParentWnd);
    }
    else
        ShowWindow(m_hWnd, SW_HIDE);

}
void CShadowUI::MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent)
{
    // The shadow algorithm:
    // Get the region of parent window,
    // Apply morphologic erosion to shrink it into the size (ShadowWndSize - Sharpness)
    // Apply modified (with blur effect) morphologic dilation to make the blurred border
    // The algorithm is optimized by assuming parent window is just "one piece" and without "wholes" on it

    // Get the region of parent window,
    // Create a full rectangle region in case of the window region is not defined
    HRGN hParentRgn = CreateRectRgn(0, 0, rcParent->right - rcParent->left, rcParent->bottom - rcParent->top);
    GetWindowRgn(hParent, hParentRgn);

    // Determine the Start and end point of each horizontal scan line
    SIZE szParent = {rcParent->right - rcParent->left, rcParent->bottom - rcParent->top};
    SIZE szShadow = {szParent.cx + 2 * m_nSize, szParent.cy + 2 * m_nSize};
    // Extra 2 lines (set to be empty) in ptAnchors are used in dilation
    int nAnchors = max(szParent.cy, szShadow.cy);    // # of anchor points pares
    int (*ptAnchors)[2] = new int[nAnchors + 2][2];
    int (*ptAnchorsOri)[2] = new int[szParent.cy][2];    // anchor points, will not modify during erosion
    ptAnchors[0][0] = szParent.cx;
    ptAnchors[0][1] = 0;
    ptAnchors[nAnchors + 1][0] = szParent.cx;
    ptAnchors[nAnchors + 1][1] = 0;
    if(m_nSize > 0)
    {
        // Put the parent window anchors at the center
        for(int i = 0; i < m_nSize; i++)
        {
            ptAnchors[i + 1][0] = szParent.cx;
            ptAnchors[i + 1][1] = 0;
            ptAnchors[szShadow.cy - i][0] = szParent.cx;
            ptAnchors[szShadow.cy - i][1] = 0;
        }
        ptAnchors += m_nSize;
    }
    for(int i = 0; i < szParent.cy; i++)
    {
        // find start point
        int j;
        for(j = 0; j < szParent.cx; j++)
        {
            if(PtInRegion(hParentRgn, j, i))
            {
                ptAnchors[i + 1][0] = j + m_nSize;
                ptAnchorsOri[i][0] = j;
                break;
            }
        }

        if(j >= szParent.cx)    // Start point not found
        {
            ptAnchors[i + 1][0] = szParent.cx;
            ptAnchorsOri[i][1] = 0;
            ptAnchors[i + 1][0] = szParent.cx;
            ptAnchorsOri[i][1] = 0;
        }
        else
        {
            // find end point
            for(j = szParent.cx - 1; j >= ptAnchors[i + 1][0]; j--)
            {
                if(PtInRegion(hParentRgn, j, i))
                {
                    ptAnchors[i + 1][1] = j + 1 + m_nSize;
                    ptAnchorsOri[i][1] = j + 1;
                    break;
                }
            }
        }
        //         if(0 != ptAnchorsOri[i][1])
        //             _RPTF2(_CRT_WARN, "%d %d\n", ptAnchorsOri[i][0], ptAnchorsOri[i][1]);
    }

    if(m_nSize > 0)
        ptAnchors -= m_nSize;    // Restore pos of ptAnchors for erosion
    int (*ptAnchorsTmp)[2] = new int[nAnchors + 2][2];    // Store the result of erosion
    // First and last line should be empty
    ptAnchorsTmp[0][0] = szParent.cx;
    ptAnchorsTmp[0][1] = 0;
    ptAnchorsTmp[nAnchors + 1][0] = szParent.cx;
    ptAnchorsTmp[nAnchors + 1][1] = 0;
    int nEroTimes = 0;
    // morphologic erosion
    for(int i = 0; i < m_nSharpness - m_nSize; i++)
    {
        nEroTimes++;
        //ptAnchorsTmp[1][0] = szParent.cx;
        //ptAnchorsTmp[1][1] = 0;
        //ptAnchorsTmp[szParent.cy + 1][0] = szParent.cx;
        //ptAnchorsTmp[szParent.cy + 1][1] = 0;
        for(int j = 1; j < nAnchors + 1; j++)
        {
            ptAnchorsTmp[j][0] = max(ptAnchors[j - 1][0], max(ptAnchors[j][0], ptAnchors[j + 1][0])) + 1;
            ptAnchorsTmp[j][1] = min(ptAnchors[j - 1][1], min(ptAnchors[j][1], ptAnchors[j + 1][1])) - 1;
        }
        // Exchange ptAnchors and ptAnchorsTmp;
        int (*ptAnchorsXange)[2] = ptAnchorsTmp;
        ptAnchorsTmp = ptAnchors;
        ptAnchors = ptAnchorsXange;
    }

    // morphologic dilation
    ptAnchors += (m_nSize < 0 ? -m_nSize : 0) + 1;    // now coordinates in ptAnchors are same as in shadow window
    // Generate the kernel
    int nKernelSize = m_nSize > m_nSharpness ? m_nSize : m_nSharpness;
    int nCenterSize = m_nSize > m_nSharpness ? (m_nSize - m_nSharpness) : 0;
    UINT32 *pKernel = new UINT32[(2 * nKernelSize + 1) * (2 * nKernelSize + 1)];
    UINT32 *pKernelIter = pKernel;
    for(int i = 0; i <= 2 * nKernelSize; i++)
    {
        for(int j = 0; j <= 2 * nKernelSize; j++)
        {
            double dLength = sqrt((i - nKernelSize) * (i - nKernelSize) + (j - nKernelSize) * (double)(j - nKernelSize));
            if(dLength < nCenterSize)
                *pKernelIter = m_nDarkness << 24 | PreMultiply(m_Color, m_nDarkness);
            else if(dLength <= nKernelSize)
            {
                UINT32 nFactor = ((UINT32)((1 - (dLength - nCenterSize) / (m_nSharpness + 1)) * m_nDarkness));
                *pKernelIter = nFactor << 24 | PreMultiply(m_Color, nFactor);
            }
            else
                *pKernelIter = 0;
            //TRACE("%d ", *pKernelIter >> 24);
            pKernelIter ++;
        }
        //TRACE("\n");
    }
    // Generate blurred border
    for(int i = nKernelSize; i < szShadow.cy - nKernelSize; i++)
    {
        int j;
        if(ptAnchors[i][0] < ptAnchors[i][1])
        {

            // Start of line
            for(j = ptAnchors[i][0];
                j < min(max(ptAnchors[i - 1][0], ptAnchors[i + 1][0]) + 1, ptAnchors[i][1]);
                j++)
            {
                for(int k = 0; k <= 2 * nKernelSize; k++)
                {
                    UINT32 *pPixel = pShadBits +
                        (szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
                    UINT32 *pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
                    for(int l = 0; l <= 2 * nKernelSize; l++)
                    {
                        if(*pPixel < *pKernelPixel)
                            *pPixel = *pKernelPixel;
                        pPixel++;
                        pKernelPixel++;
                    }
                }
            }    // for() start of line

            // End of line
            for(j = max(j, min(ptAnchors[i - 1][1], ptAnchors[i + 1][1]) - 1);
                j < ptAnchors[i][1];
                j++)
            {
                for(int k = 0; k <= 2 * nKernelSize; k++)
                {
                    UINT32 *pPixel = pShadBits +
                        (szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
                    UINT32 *pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
                    for(int l = 0; l <= 2 * nKernelSize; l++)
                    {
                        if(*pPixel < *pKernelPixel)
                            *pPixel = *pKernelPixel;
                        pPixel++;
                        pKernelPixel++;
                    }
                }
            }    // for() end of line

        }
    }    // for() Generate blurred border

    // Erase unwanted parts and complement missing
    UINT32 clCenter = m_nDarkness << 24 | PreMultiply(m_Color, m_nDarkness);
    for(int i = min(nKernelSize, max(m_nSize - m_nyOffset, 0));
        i < max(szShadow.cy - nKernelSize, min(szParent.cy + m_nSize - m_nyOffset, szParent.cy + 2 * m_nSize));
        i++)
    {
        UINT32 *pLine = pShadBits + (szShadow.cy - i - 1) * szShadow.cx;
        if(i - m_nSize + m_nyOffset < 0 || i - m_nSize + m_nyOffset >= szParent.cy)    // Line is not covered by parent window
        {
            for(int j = ptAnchors[i][0]; j < ptAnchors[i][1]; j++)
            {
                *(pLine + j) = clCenter;
            }
        }
        else
        {
            for(int j = ptAnchors[i][0];
                j < min(ptAnchorsOri[i - m_nSize + m_nyOffset][0] + m_nSize - m_nxOffset, ptAnchors[i][1]);
                j++)
                *(pLine + j) = clCenter;
            for(int j = max(ptAnchorsOri[i - m_nSize + m_nyOffset][0] + m_nSize - m_nxOffset, 0);
                j < min(ptAnchorsOri[i - m_nSize + m_nyOffset][1] + m_nSize - m_nxOffset, szShadow.cx);
                j++)
                *(pLine + j) = 0;
            for(int j = max(ptAnchorsOri[i - m_nSize + m_nyOffset][1] + m_nSize - m_nxOffset, ptAnchors[i][0]);
                j < ptAnchors[i][1];
                j++)
                *(pLine + j) = clCenter;
        }
    }

    // Delete used resources
    delete[] (ptAnchors - (m_nSize < 0 ? -m_nSize : 0) - 1);
    delete[] ptAnchorsTmp;
    delete[] ptAnchorsOri;
    delete[] pKernel;
    DeleteObject(hParentRgn);
}

void CShadowUI::ShowShadow(bool bShow)
{
    m_bIsShowShadow = bShow;
}

bool CShadowUI::IsShowShadow() const
{
    return m_bIsShowShadow;
}

bool CShadowUI::SetSize(int NewSize)
{
    if(NewSize > 20 || NewSize < -20)
        return false;

    m_nSize = (signed char)NewSize;
    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));
    return true;
}

bool CShadowUI::SetSharpness(unsigned int NewSharpness)
{
    if(NewSharpness > 20)
        return false;

    m_nSharpness = (unsigned char)NewSharpness;
    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));
    return true;
}

bool CShadowUI::SetDarkness(unsigned int NewDarkness)
{
    if(NewDarkness > 255)
        return false;

    m_nDarkness = (unsigned char)NewDarkness;
    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));
    return true;
}

bool CShadowUI::SetPosition(int NewXOffset, int NewYOffset)
{
    if(NewXOffset > 20 || NewXOffset < -20 ||
        NewYOffset > 20 || NewYOffset < -20)
        return false;
    
    m_nxOffset = (signed char)NewXOffset;
    m_nyOffset = (signed char)NewYOffset;
    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));
    return true;
}

bool CShadowUI::SetColor(COLORREF NewColor)
{
    m_Color = NewColor;
    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));
    return true;
}

bool CShadowUI::SetImage(LPCTSTR szImage)
{
    if (szImage == NULL || _tcslen(szImage) == 0)
        return false;

    m_bIsImageMode = true;
    m_sShadowImage = szImage;
    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));

    return true;
}

bool CShadowUI::SetCorner(RECT rcCorner)
{
    if (rcCorner.left < 0 || rcCorner.top < 0 || rcCorner.right < 0 || rcCorner.bottom < 0)
        return false;

    m_rcShadowCorner = rcCorner;

    if(m_hWnd != NULL && (SS_VISABLE & m_Status))
        Update(GetParent(m_hWnd));

    return true;
}

bool CShadowUI::CopyShadow(CShadowUI* pShadow)
{
    if (pShadow->GetImage()) {
        this->SetImage(pShadow->GetImage());
        this->SetCorner(pShadow->GetCorner());
    } else {
        POINT pt = pShadow->GetPosition();
        this->SetSize(pShadow->GetSize());
        this->SetSharpness(pShadow->GetSharpness());
        this->SetDarkness(pShadow->GetDarkness());
        this->SetColor(pShadow->GetColor());
        this->SetPosition(pt.x, pt.y);
    }
    this->ShowShadow(pShadow->IsShowShadow());
    return true;
}

void CShadowUI::SetStatus(int state)
{
    m_Status = state;
}

} //namespace DuiLib