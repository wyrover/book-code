#pragma once
#include <windowsx.h>
#include <tchar.h>
#include <uxtheme.h>
#include <crtdbg.h>
#include <stdlib.h>

#define SP_GRIPPER 3

// Use defered window movement API:
#define RESIZER_DEFER                   1

// used for storing window property
#define RESIZER_PROP_STRING             _T("ResizerClass")


enum ANCHOR {
    ANCHOR_DEFAULT  = 0,
    ANCHOR_LEFT     = 1,
    ANCHOR_RIGHT    = 2,
    ANCHOR_BOTH     = 3,
    ANCHOR_TOP      = ANCHOR_LEFT,
    ANCHOR_BOTTOM   = ANCHOR_RIGHT,
};


class CResizer
{
    WNDPROC m_WndProc;      // Original window proc for subclassed window
    HWND    m_hDlg;         // Handle to dialog window
    POINT   m_origSize;     // Size of dialog when first created (in template)
    POINT   m_oldSize;      // Most recent size of dialog
    POINT   m_newSize;      // Size as a result of resizing
    HDWP    m_hdwp;         // Deferred window position handle
    long    m_horzPercent;  // Horizontal breakpoint percentage
    long    m_vertPercent;  // Horizontal breakpoint percentage
    bool    m_isChild;      // Window is a control window
    bool    m_Enable;

    // Theme stuff for drawing the gripper
    typedef HTHEME(WINAPI * typeOpenThemeData)(HWND, LPCWSTR);
    typedef HRESULT(WINAPI * typeDrawThemeBackground)(HTHEME, HDC, int, int, const RECT *, const RECT *);
    typedef HRESULT(WINAPI * typeCloseThemeData)(HTHEME);
    typeOpenThemeData       m_pOpenThemeData;
    typeDrawThemeBackground m_pDrawThemeBackground;
    typeCloseThemeData      m_pCloseThemeData;
    HTHEME                  m_hTheme;

    // structure for overriding an individual child window's behavior
    struct OVERRIDE {
        double  leftAnchor;
        double  rightAnchor;
        double  topAnchor;
        double  bottomAnchor;
        RECT    origPos;
        HWND    hChild;
    }   *   m_OverrideList; // Overrides for default anchor points
    int     m_OverrideCnt;

    // Get child window location
    void GetChildPosition(HWND hChild, RECT * rc)
    {
        GetWindowRect(hChild, rc);
        ScreenToClient(m_hDlg, (POINT *)&rc->left);
        ScreenToClient(m_hDlg, (POINT *)&rc->right);
    }

    // Return the override associated with a given child window
    OVERRIDE * FindOverride(HWND hChild, bool create)
    {
        if (hChild == NULL)
            return NULL;

        for (int i = 0; i < m_OverrideCnt; ++i)
            if (m_OverrideList[i].hChild == hChild)
                return m_OverrideList + i;

        if (! create)
            return NULL;

        // create item
        m_OverrideList = (OVERRIDE *)realloc(m_OverrideList, ++m_OverrideCnt * sizeof m_OverrideList[0]);
        OVERRIDE * override = m_OverrideList + m_OverrideCnt - 1;
        override->hChild        = hChild;
        override->leftAnchor    = -1.0;
        override->rightAnchor   = -1.0;
        override->topAnchor     = -1.0;
        override->bottomAnchor  = -1.0;
        GetChildPosition(hChild, &override->origPos);
        return override;
    }

    static BOOL CALLBACK ResizeCallback(HWND hChild, LPARAM lParam)
    {
        bool        ResizeOkay  = true;
        CResizer *  This        = (CResizer *) lParam;
#if RESIZER_DEFER

        // Ensure child is a direct descendent of parent (mandatory for DeferWindowPos)
        if (GetParent(hChild) != This->m_hDlg)
            return TRUE;

#endif
        // Get child window location
        RECT    rcOrig;
        This->GetChildPosition(hChild, &rcOrig);
        // Select a "center" for dividing screen into quadrants
        int horzBreak = This->m_oldSize.x * This->m_horzPercent / 100;
        int vertBreak = This->m_oldSize.y * This->m_vertPercent / 100;
        // Compute default anchor points
        double leftAnchor   = rcOrig.left   < horzBreak ? 0.0 : 1.0;
        double rightAnchor  = rcOrig.right  < horzBreak ? 0.0 : 1.0;
        double topAnchor    = rcOrig.top    < vertBreak ? 0.0 : 1.0;
        double bottomAnchor = rcOrig.bottom < vertBreak ? 0.0 : 1.0;
        // Apply window class override
#if _DEBUG
        TCHAR Text[ MAX_PATH ] = TEXT("");
        GetWindowText(hChild, Text, MAX_PATH);
#endif
        TCHAR Class[ MAX_PATH ] = TEXT("");
        GetClassName(hChild, Class, MAX_PATH);

        if (_tcsicmp(Class, TEXT("Button")) == 0)  {
            DWORD Style = GetWindowLong(hChild, GWL_STYLE);
            Style &= 0xF;

            if (Style == BS_PUSHBUTTON  ||  Style == BS_DEFPUSHBUTTON  ||  Style == BS_OWNERDRAW  ||  Style == BS_USERBUTTON)
                ResizeOkay = false;
        }

        if (_tcsicmp(Class, TEXT("STATIC")) == 0)  {
            // force static text to redraw
            InvalidateRect(hChild, NULL, false);
        }

        if (! ResizeOkay)  {
            // Not allowed to resize this control, so prevent the anchors from being different
            if (horzBreak - rcOrig.left >= rcOrig.right - horzBreak)
                rightAnchor = leftAnchor;
            else
                leftAnchor = rightAnchor;

            if (vertBreak - rcOrig.top >= rcOrig.bottom - vertBreak)
                bottomAnchor = topAnchor;
            else
                topAnchor = bottomAnchor;
        }

        // Apply user override
        const OVERRIDE * override = This->FindOverride(hChild, false);

        if (override)  {
            if (override->leftAnchor    >= 0.0)    leftAnchor      = override->leftAnchor;

            if (override->rightAnchor   >= 0.0)    rightAnchor     = override->rightAnchor;

            if (override->topAnchor >= 0.0)    topAnchor       = override->topAnchor;

            if (override->bottomAnchor >= 0.0) bottomAnchor    = override->bottomAnchor;
        }

        // adjust size based on anchor points
        RECT    rcNew;
        rcNew.left  = leftAnchor   == 0.0 ? rcOrig.left
                      : leftAnchor   == 1.0 ? rcOrig.left     +  This->m_newSize.x - This->m_oldSize.x
                      : (long)(override->origPos.left         + (This->m_newSize.x - This->m_origSize.x) * override->leftAnchor);
        rcNew.right = rightAnchor  == 0.0 ? rcOrig.right
                      : rightAnchor  == 1.0 ? rcOrig.right    +  This->m_newSize.x - This->m_oldSize.x
                      : (long)(override->origPos.right        + (This->m_newSize.x - This->m_origSize.x) * override->rightAnchor);
        rcNew.top   = topAnchor    == 0.0 ? rcOrig.top
                      : topAnchor    == 1.0 ? rcOrig.top      +  This->m_newSize.y - This->m_oldSize.y
                      : (long)(override->origPos.top          + (This->m_newSize.y - This->m_origSize.y) * override->topAnchor);
        rcNew.bottom = bottomAnchor == 0.0 ? rcOrig.bottom
                       : bottomAnchor == 1.0 ? rcOrig.bottom   +  This->m_newSize.y - This->m_oldSize.y
                       : (long)(override->origPos.bottom       + (This->m_newSize.y - This->m_origSize.y) * override->bottomAnchor);
        // Update position relative to parent, which may be different than dialog
#if RESIZER_DEFER
        This->m_hdwp = DeferWindowPos(This->m_hdwp, hChild, NULL, rcNew.left, rcNew.top, rcNew.right - rcNew.left, rcNew.bottom - rcNew.top, SWP_NOZORDER);
#else
        MapWindowPoints(This->m_hDlg, GetParent(hChild), (POINT *)&rcNew, 2);
        MoveWindow(hChild, rcNew.left, rcNew.top, rcNew.right - rcNew.left, rcNew.bottom - rcNew.top, false);
#endif
#if 0
        // If the window grew we need to invalidate the new portion
        RECT    rcInvalid;
        SubtractRect(&rcInvalid, &rcNew, &rcOrig);
        MapWindowPoints(GetParent(hChild), hChild, (POINT *)&rcNew, 2);
        MapWindowPoints(GetParent(hChild), hChild, (POINT *)&rcInvalid, 2);
        InvalidateRect(hChild, &rcInvalid, true);
#endif
        return TRUE;
    }

    void ResizeDialog()
    {
        // All of this code assumes that windows only grow, not shrink, from their original size:
        _ASSERT(m_newSize.x >= m_origSize.x  &&  m_newSize.y >= m_origSize.y);

        if (m_oldSize.y != m_newSize.y  ||  m_oldSize.x != m_newSize.x)  {
            // move all windows
#if RESIZER_DEFER
            m_hdwp = BeginDeferWindowPos(20);
#endif
            EnumChildWindows(m_hDlg, ResizeCallback, (LPARAM)this);
#if RESIZER_DEFER
            EndDeferWindowPos(m_hdwp);
#endif

            // Invalidate gripper
            if (!m_isChild)  {
                RECT    rcClient;
                GetClientRect(m_hDlg, &rcClient);
                int cx = GetSystemMetrics(SM_CXHSCROLL);
                int cy = GetSystemMetrics(SM_CYVSCROLL);
                RECT    rcNewGripper = { rcClient.right - cx, rcClient.bottom - cy, rcClient.right, rcClient.bottom };
                int dx = m_oldSize.x - m_newSize.x;
                int dy = m_oldSize.y - m_newSize.y;
                RECT    rcOldGripper = rcNewGripper;
                OffsetRect(&rcOldGripper, dx, dy);
                RECT    rcInvalid;
                UnionRect(&rcInvalid, &rcOldGripper, &rcNewGripper);
                InvalidateRect(m_hDlg, &rcInvalid, true);
            }

            // save current size for next time
            m_oldSize = m_newSize;
        }
    }

    static inline CResizer * GetThis(HWND hWnd)
    {
        return (CResizer *) GetProp(hWnd, RESIZER_PROP_STRING);
    }
    static inline void SetThis(HWND hWnd, CResizer * This)
    {
        SetProp(hWnd, RESIZER_PROP_STRING, (HANDLE)This);
    }

    static LRESULT CALLBACK SubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        CResizer * This = GetThis(hWnd);

        switch (message) {
        case WM_GETMINMAXINFO: {
            MINMAXINFO * minmax = (MINMAXINFO *) lParam;
            minmax->ptMinTrackSize = This->m_origSize;
        }

        return 0;

        case WM_SIZE:
            if (! This->m_Enable)
                break;

            if (wParam == SIZE_MAXIMIZED  ||  wParam == SIZE_RESTORED)  {
                RECT    rc;

                if (! GetWindowRect(hWnd, &rc))
                    break;

                This->m_newSize.x = rc.right  - rc.left;
                This->m_newSize.y = rc.bottom - rc.top;
                This->ResizeDialog();
            }

            break;

        case WM_NCHITTEST: {
            // check whether cursor is in size grip
            LRESULT ht = CallWindowProc(This->m_WndProc, hWnd, message, wParam, lParam);

            // if we're a control widget we're done
            if (This->m_isChild)
                return ht;

            if (ht == HTCLIENT)  {
                if (! IsZoomed(hWnd))  {
                    RECT rc;
                    GetWindowRect(hWnd, &rc);
                    rc.left = rc.right  - GetSystemMetrics(SM_CXHSCROLL);
                    rc.top  = rc.bottom - GetSystemMetrics(SM_CYVSCROLL);
                    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

                    if (PtInRect(&rc, pt))
                        return HTBOTTOMRIGHT;
                }
            }

            return ht;
        }
        break;

        case WM_PAINT: {
            // if we're a control widget we're done
            if (This->m_isChild)
                break;

            if (! IsZoomed(hWnd))  {
                // Paint size grip
                CResizer    *   This = GetThis(hWnd);
                PAINTSTRUCT     ps;
                HDC             hdc = BeginPaint(hWnd, &ps);
                RECT            rc;
                GetClientRect(hWnd, &rc);
                rc.left = rc.right  - GetSystemMetrics(SM_CXHSCROLL);
                rc.top  = rc.bottom - GetSystemMetrics(SM_CYVSCROLL);

                if (This->m_hTheme)
                    This->m_pDrawThemeBackground(This->m_hTheme, hdc, SP_GRIPPER, 0, &rc, NULL);
                else
                    DrawFrameControl(hdc, &rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);

                EndPaint(hWnd, &ps);
            }
        }
        break;

        case WM_DESTROY: {
            // restore original window proc
            WNDPROC proc = This->m_WndProc;
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)proc);
            // delete ourself
            delete This;
            // invoke original proc
            return CallWindowProc(proc, hWnd, message, wParam, lParam);
        }
        break;
        }

        return CallWindowProc(This->m_WndProc, hWnd, message, wParam, lParam);
    }

public:

    CResizer(HWND hWnd = NULL)
    {
        m_horzPercent   = 50;
        m_vertPercent   = 67;
        m_OverrideList  = NULL;
        m_OverrideCnt   = 0;
        m_Enable        = true;

        if (hWnd)  {
            Attach(hWnd);
        }
    }

    static inline CResizer * GetResizer(HWND hWnd)
    {
        return GetThis(hWnd);
    }

    void Attach(HWND hWnd)
    {
        m_hDlg = hWnd;
        // store our data with window
        SetThis(hWnd, this);
        // Subclass window
        m_WndProc = (WNDPROC) GetWindowLongPtr(hWnd, GWLP_WNDPROC);
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)SubclassProc);
        HMODULE hLib = LoadLibrary(TEXT("uxtheme.dll"));
        m_pOpenThemeData        = (typeOpenThemeData)       GetProcAddress(hLib, "OpenThemeData");
        m_pCloseThemeData       = (typeCloseThemeData)      GetProcAddress(hLib, "CloseThemeData");
        m_pDrawThemeBackground  = (typeDrawThemeBackground) GetProcAddress(hLib, "DrawThemeBackground");
        m_hTheme                = m_pOpenThemeData ? m_pOpenThemeData(m_hDlg, L"status") : NULL;
        // Get initial window size
        RECT    rc;
        GetWindowRect(m_hDlg, &rc);
        m_oldSize.x = rc.right  - rc.left;
        m_oldSize.y = rc.bottom - rc.top;
        m_origSize = m_oldSize;
        m_isChild = (GetWindowStyle(hWnd) & WS_CHILD) != 0;
    }

    ~CResizer()
    {
        Reset();

        if (m_pCloseThemeData) m_pCloseThemeData(m_hTheme);
    }

    void Enable(bool enable)
    {
        m_Enable = enable;
    }

    void Reset()
    {
        // only called if resizer is being reused
        m_hDlg = NULL;
        free(m_OverrideList);
        m_OverrideList = NULL;
        m_OverrideCnt  = 0;
    }

    void SetBreakPercents(long horzPercent, long vertPercent)
    {
        m_horzPercent   = horzPercent;
        m_vertPercent   = vertPercent;
    }

    // Called to manually resize the window
    POINT Resize(const POINT * pNewSize)
    {
        if (pNewSize == NULL)    // if size not specified use original size
            pNewSize = &m_origSize;

        POINT curSize = m_oldSize;
        SetWindowPos(m_hDlg, NULL, 0, 0, pNewSize->x, pNewSize->y, SWP_NOREDRAW | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE);
        return curSize;
    }

    // Called to explicitly set the anchors for a child window
    void SetHorz(HWND hChild, ANCHOR horzAnchor)
    {
        if (hChild == NULL)  {
            _ASSERT(false);
            return;
        }

        OVERRIDE * override = FindOverride(hChild, true);

        switch (horzAnchor)  {
        case ANCHOR_DEFAULT:
            break;

        case ANCHOR_LEFT:
            override->leftAnchor  = 0.0;
            override->rightAnchor = 0.0;
            break;

        case ANCHOR_RIGHT:
            override->leftAnchor  = 1.0;
            override->rightAnchor = 1.0;
            break;

        case ANCHOR_BOTH:
            override->leftAnchor  = 0.0;
            override->rightAnchor = 1.0;
            break;
        }
    }
    void SetVert(HWND hChild, ANCHOR vertAnchor)
    {
        if (hChild == NULL)  {
            _ASSERT(false);
            return;
        }

        OVERRIDE * override = FindOverride(hChild, true);

        switch (vertAnchor)  {
        case ANCHOR_DEFAULT:
            break;

        case ANCHOR_TOP:
            override->topAnchor  = 0.0;
            override->bottomAnchor = 0.0;
            break;

        case ANCHOR_BOTTOM:
            override->topAnchor  = 1.0;
            override->bottomAnchor = 1.0;
            break;

        case ANCHOR_BOTH:
            override->topAnchor  = 0.0;
            override->bottomAnchor = 1.0;
            break;
        }
    }
    void SetBoth(HWND hChild, ANCHOR horzAnchor, ANCHOR vertAnchor)
    {
        SetHorz(hChild, horzAnchor);
        SetVert(hChild, vertAnchor);
    }



    void SetHorz(HWND hChild, double left, double right)
    {
        _ASSERT(left >= 0.0  &&  right <= 1.0  &&  left <= right);

        if (right >= 0.99999)
            right = 1.0;

        if (hChild)  {
            OVERRIDE * override = FindOverride(hChild, true);
            override->leftAnchor = left;
            override->rightAnchor = right;
        }
    }
    void SetVert(HWND hChild, double top, double bottom)
    {
        _ASSERT(top >= 0.0  &&  bottom <= 1.0  &&  top <= bottom);

        if (bottom >= 0.99999)
            bottom = 1.0;

        OVERRIDE * override = FindOverride(hChild, true);
        override->topAnchor = top;
        override->bottomAnchor = bottom;
    }

    // Set a list of child windows to grow/shrink proportionally
    void SetHorzProportional(int cnt, HWND * ChildList)
    {
        double width = 0.0;

        for (int i = 0; i < cnt; ++i)  {
            RECT    rc;
            GetWindowRect(ChildList[i], &rc);
            width += rc.right - rc.left;
        }

        double pos = 0.0;

        for (int i = 0; i < cnt; ++i)  {
            RECT    rc;
            GetWindowRect(ChildList[i], &rc);
            double delta = (rc.right - rc.left) / width;
            SetHorz(ChildList[i], pos, pos + delta);
            pos += delta;
        }
    }
    // Set a list of child windows to grow/shrink proportionally
    void SetVertProportional(int cnt, HWND * ChildList)
    {
        double height = 0.0;

        for (int i = 0; i < cnt; ++i)  {
            RECT    rc;
            GetWindowRect(ChildList[i], &rc);
            height += rc.bottom - rc.top;
        }

        double pos = 0.0;

        for (int i = 0; i < cnt; ++i)  {
            RECT    rc;
            GetWindowRect(ChildList[i], &rc);
            double delta = (rc.bottom - rc.top) / height;
            SetVert(ChildList[i], pos, pos + delta);
            pos += delta;
        }
    }

    // Set a set of child windows to grow/shrink proportionally
    void SetHorzProportional(HWND h1)
    {
        HWND list[] = { h1 };
        SetHorzProportional(sizeof list / sizeof list[0], list);
    }
    void SetHorzProportional(HWND h1, HWND h2)
    {
        HWND list[] = { h1, h2 };
        SetHorzProportional(sizeof list / sizeof list[0], list);
    }
    void SetHorzProportional(HWND h1, HWND h2, HWND h3)
    {
        HWND list[] = { h1, h2, h3 };
        SetHorzProportional(sizeof list / sizeof list[0], list);
    }
    void SetHorzProportional(HWND h1, HWND h2, HWND h3, HWND h4)
    {
        HWND list[] = { h1, h2, h3, h4 };
        SetHorzProportional(sizeof list / sizeof list[0], list);
    }

    // Set a set of child windows to grow/shrink proportionally
    void SetVertProportional(HWND h1)
    {
        HWND list[] = { h1 };
        SetVertProportional(sizeof list / sizeof list[0], list);
    }
    void SetVertProportional(HWND h1, HWND h2)
    {
        HWND list[] = { h1, h2 };
        SetVertProportional(sizeof list / sizeof list[0], list);
    }
    void SetVertProportional(HWND h1, HWND h2, HWND h3)
    {
        HWND list[] = { h1, h2, h3 };
        SetVertProportional(sizeof list / sizeof list[0], list);
    }
    void SetVertProportional(HWND h1, HWND h2, HWND h3, HWND h4)
    {
        HWND list[] = { h1, h2, h3, h4 };
        SetVertProportional(sizeof list / sizeof list[0], list);
    }
};
