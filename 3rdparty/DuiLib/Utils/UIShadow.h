#ifndef __UISHADOW_H__
#define __UISHADOW_H__

#pragma once
#include "map"

namespace DuiLib
{

class UILIB_API CShadowUI
{
public:
    friend class CPaintManagerUI;

    CShadowUI(void);
    virtual ~CShadowUI(void);

public:
    typedef BOOL (WINAPI *pfnUpdateLayeredWindow)(HWND hWnd, HDC hdcDst, POINT *pptDst,
        SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
    typedef HRESULT (WINAPI *pfnDwmIsCompositionEnabled)(BOOL *pfEnabled);

    void ShowShadow(bool bShow);    
    bool IsShowShadow() const;

    bool SetSize(int NewSize = 0);
    bool SetSharpness(unsigned int NewSharpness = 5);
    bool SetDarkness(unsigned int NewDarkness = 200);
    bool SetPosition(int NewXOffset = 5, int NewYOffset = 5);
    bool SetColor(COLORREF NewColor = 0);

    bool SetImage(LPCTSTR szImage);
    bool SetCorner(RECT rcCorner);
    
    int GetSize() const {return m_nSize;}
    int GetSharpness() const {return m_nSharpness;}
    int GetDarkness() const {return m_nDarkness;}
    POINT GetPosition() const { POINT pt = {m_nxOffset, m_nyOffset}; return pt;};
    COLORREF GetColor() const {return m_Color;}

    LPCTSTR GetImage() const {return m_sShadowImage.c_str();}
    RECT GetCorner() const {return m_rcShadowCorner;}
    bool CopyShadow(CShadowUI* pShadow);
    void Create(HWND hWndParent, CPaintManagerUI* pPaintManager);
    LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:

    static bool Initialize(HINSTANCE hInstance);
    static std::map<HWND, CShadowUI *>& GetShadowMap();
    void Update(HWND hParent);
    void MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent);
    void Show(HWND hParent); 

    inline DWORD PreMultiply(COLORREF cl, unsigned char nAlpha)
    {
        return (GetRValue(cl) * (DWORD)nAlpha / 255) |
            (GetGValue(cl) * (DWORD)nAlpha / 255) << 8 |
            (GetBValue(cl) * (DWORD)nAlpha / 255) << 16 ;
    }
    
protected:
    enum ShadowStatus
    {
        SS_ENABLED = 1,                // Shadow is enabled, if not, the following one is always false
        SS_VISABLE = 1 << 1,        // Shadow window is visible
        SS_PARENTVISIBLE = 1<< 2,    // Parent window is visible, if not, the above one is always false
        SS_DISABLEDBYAERO    = 1 << 3    // Shadow is enabled, but do not show because areo is enabled
    };
    void SetStatus(int state);


/*    static std::map<HWND, CShadowUI *> *s_Shadowmap;*/
    static bool s_bHasInit;

    static bool s_bVista;
    static pfnUpdateLayeredWindow        s_UpdateLayeredWindow;
    static pfnDwmIsCompositionEnabled    s_DwmIsCompositionEnabled;

    LPARAM m_WndSize;
    bool m_bUpdate;
    COLORREF m_Color;

private:
    unsigned char m_nDarkness;
    unsigned char m_nSharpness;
    signed char m_nSize;

    CPaintManagerUI    *m_pManager;
    HWND             m_hWnd;
    LONG             m_OriParentProc;
    BYTE             m_Status;
    bool             m_bIsImageMode;
    bool             m_bIsShowShadow;
    RECT            m_rcShadowCorner;
    std::wstring    m_sShadowImage;
    signed char     m_nxOffset;
    signed char     m_nyOffset;
};

}

#endif //__UISHADOW_H__