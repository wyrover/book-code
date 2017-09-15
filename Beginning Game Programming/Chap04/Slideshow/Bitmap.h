//-----------------------------------------------------------------
// Bitmap Object
// C++ Header - Bitmap.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>

//-----------------------------------------------------------------
// Bitmap Class
//-----------------------------------------------------------------
class Bitmap
{
protected:
    // Member Variables
    HBITMAP m_hBitmap;
    int     m_iWidth, m_iHeight;

    // Helper Methods
    void Free();

public:
    // Constructor(s)/Destructor
    Bitmap();
    Bitmap(HDC hDC, LPTSTR szFileName);
    Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);
    Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
    virtual ~Bitmap();

    // General Methods
    BOOL Create(HDC hDC, LPTSTR szFileName);
    BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
    BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
    void Draw(HDC hDC, int x, int y);
    int  GetWidth()
    {
        return m_iWidth;
    };
    int  GetHeight()
    {
        return m_iHeight;
    };
};
