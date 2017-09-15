//-----------------------------------------------------------------
// Bitmap Object
// C++ Source - Bitmap.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Bitmap.h"

//-----------------------------------------------------------------
// Bitmap Constructor(s)/Destructor
//-----------------------------------------------------------------
Bitmap::Bitmap()
    : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
}

// Create a bitmap from a file
Bitmap::Bitmap(HDC hDC, LPTSTR szFileName)
    : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
    Create(hDC, szFileName);
}

// Create a bitmap from a resource
Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance)
    : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
    Create(hDC, uiResID, hInstance);
}

// Create a blank bitmap from scratch
Bitmap::Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
    : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
    Create(hDC, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap()
{
    Free();
}

//-----------------------------------------------------------------
// Bitmap Helper Methods
//-----------------------------------------------------------------
void Bitmap::Free()
{
    // Delete the bitmap graphics object
    if (m_hBitmap != NULL) {
        DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
    }
}

//-----------------------------------------------------------------
// Bitmap General Methods
//-----------------------------------------------------------------
BOOL Bitmap::Create(HDC hDC, LPTSTR szFileName)
{
    // Free any previous bitmap info
    Free();
    // Open the bitmap file
    HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    // Read the bitmap file header
    BITMAPFILEHEADER  bmfHeader;
    DWORD             dwBytesRead;
    BOOL bOK = ReadFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER),
                        &dwBytesRead, NULL);

    if ((!bOK) || (dwBytesRead != sizeof(BITMAPFILEHEADER)) ||
        (bmfHeader.bfType != 0x4D42)) {
        CloseHandle(hFile);
        return FALSE;
    }

    BITMAPINFO* pBitmapInfo = (new BITMAPINFO);

    if (pBitmapInfo != NULL) {
        // Read the bitmap info header
        bOK = ReadFile(hFile, pBitmapInfo, sizeof(BITMAPINFOHEADER),
                       &dwBytesRead, NULL);

        if ((!bOK) || (dwBytesRead != sizeof(BITMAPINFOHEADER))) {
            CloseHandle(hFile);
            Free();
            return FALSE;
        }

        // Store the width and height of the bitmap
        m_iWidth = (int)pBitmapInfo->bmiHeader.biWidth;
        m_iHeight = (int)pBitmapInfo->bmiHeader.biHeight;
        // Get a handle to the bitmap and copy the image bits
        PBYTE pBitmapBits;
        m_hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS,
                                     (PVOID*)&pBitmapBits, NULL, 0);

        if ((m_hBitmap != NULL) && (pBitmapBits != NULL)) {
            SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
            bOK = ReadFile(hFile, pBitmapBits, pBitmapInfo->bmiHeader.biSizeImage,
                           &dwBytesRead, NULL);

            if (bOK)
                return TRUE;
        }
    }

    // Something went wrong, so cleanup everything
    Free();
    return FALSE;
}

BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
    // Free any previous DIB info
    Free();
    // Find the bitmap resource
    HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(uiResID), RT_BITMAP);

    if (hResInfo == NULL)
        return FALSE;

    // Load the bitmap resource
    HGLOBAL hMemBitmap = LoadResource(hInstance, hResInfo);

    if (hMemBitmap == NULL)
        return FALSE;

    // Lock the resource and access the entire bitmap image
    PBYTE pBitmapImage = (BYTE*)LockResource(hMemBitmap);

    if (pBitmapImage == NULL) {
        FreeResource(hMemBitmap);
        return FALSE;
    }

    // Store the width and height of the bitmap
    BITMAPINFO* pBitmapInfo = (BITMAPINFO*)pBitmapImage;
    m_iWidth = (int)pBitmapInfo->bmiHeader.biWidth;
    m_iHeight = (int)pBitmapInfo->bmiHeader.biHeight;
    // Get a handle to the bitmap and copy the image bits
    PBYTE pBitmapBits;
    m_hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS,
                                 (PVOID*)&pBitmapBits, NULL, 0);

    if ((m_hBitmap != NULL) && (pBitmapBits != NULL)) {
        const PBYTE pTempBits = pBitmapImage + pBitmapInfo->bmiHeader.biSize +
                                pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
        CopyMemory(pBitmapBits, pTempBits, pBitmapInfo->bmiHeader.biSizeImage);
        // Unlock and free the bitmap graphics object
        UnlockResource(hMemBitmap);
        FreeResource(hMemBitmap);
        return TRUE;
    }

    // Something went wrong, so cleanup everything
    UnlockResource(hMemBitmap);
    FreeResource(hMemBitmap);
    Free();
    return FALSE;
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
    // Create a blank bitmap
    m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);

    if (m_hBitmap == NULL)
        return FALSE;

    // Set the width and height
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    // Create a memory device context to draw on the bitmap
    HDC hMemDC = CreateCompatibleDC(hDC);
    // Create a solid brush to fill the bitmap
    HBRUSH hBrush = CreateSolidBrush(crColor);
    // Select the bitmap into the device context
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
    // Fill the bitmap with a solid color
    RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
    FillRect(hMemDC, &rcBitmap, hBrush);
    // Cleanup
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBrush);
    return TRUE;
}

void Bitmap::Draw(HDC hDC, int x, int y)
{
    if (m_hBitmap != NULL) {
        // Create a memory device context for the bitmap
        HDC hMemDC = CreateCompatibleDC(hDC);
        // Select the bitmap into the device context
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
        // Draw the bitmap to the destination device context
        BitBlt(hDC, x, y, GetWidth(), GetHeight(), hMemDC, 0, 0, SRCCOPY);
        // Restore and delete the memory device context
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}
