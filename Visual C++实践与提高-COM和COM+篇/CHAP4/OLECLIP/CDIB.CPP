// cdib.cpp
// new version for WIN32
#include "stdafx.h"
#include "cdib.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CDib, CObject, 0);

CDib::CDib()
{
    m_hFile = NULL;
    m_hBitmap = NULL;
    m_hPalette = NULL;
    m_nBmihAlloc = m_nImageAlloc = noAlloc;
    Empty();
}

CDib::CDib(CSize size, int nBitCount)
{
    m_hFile = NULL;
    m_hBitmap = NULL;
    m_hPalette = NULL;
    m_nBmihAlloc = m_nImageAlloc = noAlloc;
    Empty();
    ComputePaletteSize(nBitCount);
    m_lpBMIH = (LPBITMAPINFOHEADER) new
               char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries];
    m_nBmihAlloc = crtAlloc;
    m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
    m_lpBMIH->biWidth = size.cx;
    m_lpBMIH->biHeight = size.cy;
    m_lpBMIH->biPlanes = 1;
    m_lpBMIH->biBitCount = nBitCount;
    m_lpBMIH->biCompression = BI_RGB;
    m_lpBMIH->biSizeImage = 0;
    m_lpBMIH->biXPelsPerMeter = 0;
    m_lpBMIH->biYPelsPerMeter = 0;
    m_lpBMIH->biClrUsed = m_nColorTableEntries;
    m_lpBMIH->biClrImportant = m_nColorTableEntries;
    ComputeMetrics();
    memset(m_lpvColorTable, 0, sizeof(RGBQUAD) * m_nColorTableEntries);
    m_lpImage = NULL;  // no data yet
}

CDib::~CDib()
{
    Empty();
}

CSize CDib::GetDimensions()
{
    if (m_lpBMIH == NULL) return CSize(0, 0);

    return CSize((int) m_lpBMIH->biWidth, (int) m_lpBMIH->biHeight);
}

BOOL CDib::AttachMapFile(const char* strPathname, BOOL bShare) // for reading
{
    // if we open the same file twice, Windows treats it as 2 separate files
    // doesn't work with rare BMP files where # palette entries > biClrUsed
    HANDLE hFile = ::CreateFile(strPathname, GENERIC_WRITE | GENERIC_READ,
                                bShare ? FILE_SHARE_READ : 0,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ASSERT(hFile != INVALID_HANDLE_VALUE);
    DWORD dwFileSize = ::GetFileSize(hFile, NULL);
    HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    DWORD dwErr = ::GetLastError();

    if (hMap == NULL) {
        AfxMessageBox("Empty bitmap file");
        return FALSE;
    }

    LPVOID lpvFile = ::MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0); // map whole file
    ASSERT(lpvFile != NULL);

    if (((LPBITMAPFILEHEADER) lpvFile)->bfType != 0x4d42) {
        AfxMessageBox("Invalid bitmap file");
        DetachMapFile();
        return FALSE;
    }

    AttachMemory((LPBYTE) lpvFile + sizeof(BITMAPFILEHEADER));
    m_lpvFile = lpvFile;
    m_hFile = hFile;
    m_hMap = hMap;
    return TRUE;
}

BOOL CDib::CopyToMapFile(const char* strPathname)
{
    // copies DIB to a new file, releases prior pointers
    // if you previously used CreateSection, the HBITMAP will be NULL (and unusable)
    BITMAPFILEHEADER bmfh;
    bmfh.bfType = 0x4d42;  // 'BM'
    bmfh.bfSize = m_dwSizeImage + sizeof(BITMAPINFOHEADER) +
                  sizeof(RGBQUAD) * m_nColorTableEntries + sizeof(BITMAPFILEHEADER);
    // meaning of bfSize open to interpretation
    bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                     sizeof(RGBQUAD) * m_nColorTableEntries;
    HANDLE hFile = ::CreateFile(strPathname, GENERIC_WRITE | GENERIC_READ, 0, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    ASSERT(hFile != INVALID_HANDLE_VALUE);
    int nSize =  sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                 sizeof(RGBQUAD) * m_nColorTableEntries +  m_dwSizeImage;
    HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, nSize, NULL);
    DWORD dwErr = ::GetLastError();
    ASSERT(hMap != NULL);
    LPVOID lpvFile = ::MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0); // map whole file
    ASSERT(lpvFile != NULL);
    LPBYTE lpbCurrent = (LPBYTE) lpvFile;
    memcpy(lpbCurrent, &bmfh, sizeof(BITMAPFILEHEADER)); // file header
    lpbCurrent += sizeof(BITMAPFILEHEADER);
    LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) lpbCurrent;
    memcpy(lpbCurrent, m_lpBMIH,
           sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries); // info
    lpbCurrent += sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
    memcpy(lpbCurrent, m_lpImage, m_dwSizeImage); // bit image
    DWORD dwSizeImage = m_dwSizeImage;
    Empty();
    m_dwSizeImage = dwSizeImage;
    m_nBmihAlloc = m_nImageAlloc = noAlloc;
    m_lpBMIH = lpBMIH;
    m_lpImage = lpbCurrent;
    m_hFile = hFile;
    m_hMap = hMap;
    m_lpvFile = lpvFile;
    ComputePaletteSize(m_lpBMIH->biBitCount);
    ComputeMetrics();
    MakePalette();
    return TRUE;
}

BOOL CDib::AttachMemory(LPVOID lpvMem, BOOL bMustDelete, HGLOBAL hGlobal)
{
    // assumes contiguous BITMAPINFOHEADER, color table, image
    // color table could be zero length
    Empty();
    m_hGlobal = hGlobal;

    if (bMustDelete == FALSE) {
        m_nBmihAlloc = noAlloc;
    } else {
        m_nBmihAlloc = ((hGlobal == NULL) ? crtAlloc : heapAlloc);
    }

    try {
        m_lpBMIH = (LPBITMAPINFOHEADER) lpvMem;
        ComputeMetrics();
        ComputePaletteSize(m_lpBMIH->biBitCount);
        m_lpImage = (LPBYTE) m_lpvColorTable + sizeof(RGBQUAD) * m_nColorTableEntries;
        MakePalette();
    } catch (CException* pe) {
        AfxMessageBox("AttachMemory error");
        pe->Delete();
        return FALSE;
    }

    return TRUE;
}

UINT CDib::UsePalette(CDC* pDC, BOOL bBackground /* = FALSE */)
{
    if (m_hPalette == NULL) return 0;

    HDC hdc = pDC->GetSafeHdc();
    ::SelectPalette(hdc, m_hPalette, bBackground);
    return ::RealizePalette(hdc);
}

BOOL CDib::Draw(CDC* pDC, CPoint origin, CSize size)
{
    if (m_lpBMIH == NULL) return FALSE;

    if (m_hPalette != NULL) {
        ::SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
    }

    pDC->SetStretchBltMode(COLORONCOLOR);
    ::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
                    0, 0, m_lpBMIH->biWidth, m_lpBMIH->biHeight,
                    m_lpImage, (LPBITMAPINFO) m_lpBMIH, DIB_RGB_COLORS, SRCCOPY);
    return TRUE;
}

HBITMAP CDib::CreateSection(CDC* pDC /* = NULL */)
{
    if (m_lpBMIH == NULL) return NULL;

    if (m_lpImage != NULL) return NULL; // can only do this if image doesn't exist

    m_hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO) m_lpBMIH,
                                   DIB_RGB_COLORS, (LPVOID*) &m_lpImage, NULL, 0);
    ASSERT(m_lpImage != NULL);
    return m_hBitmap;
}

BOOL CDib::MakePalette()
{
    // makes a logical palette (m_hPalette) from the DIB's color table
    // this palette will be selected and realized prior to drawing the DIB
    if (m_nColorTableEntries == 0) return FALSE;

    if (m_hPalette != NULL) ::DeleteObject(m_hPalette);

    TRACE("CDib::MakePalette -- m_nColorTableEntries = %d\n", m_nColorTableEntries);
    LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
                           m_nColorTableEntries * sizeof(PALETTEENTRY)];
    pLogPal->palVersion = 0x300;
    pLogPal->palNumEntries = m_nColorTableEntries;
    LPRGBQUAD pDibQuad = (LPRGBQUAD) m_lpvColorTable;

    for (int i = 0; i < m_nColorTableEntries; i++) {
        pLogPal->palPalEntry[i].peRed = pDibQuad->rgbRed;
        pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
        pLogPal->palPalEntry[i].peBlue = pDibQuad->rgbBlue;
        pLogPal->palPalEntry[i].peFlags = 0;
        pDibQuad++;
    }

    m_hPalette = ::CreatePalette(pLogPal);
    delete pLogPal;
    return TRUE;
}

BOOL CDib::SetSystemPalette(CDC* pDC)
{
    // if the DIB doesn't have a color table, we can use the system's halftone palette
    if (m_nColorTableEntries != 0) return FALSE;

    m_hPalette = ::CreateHalftonePalette(pDC->GetSafeHdc());
    return TRUE;
}

HBITMAP CDib::CreateBitmap(CDC* pDC)
{
    if (m_dwSizeImage == 0) return NULL;

    HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), m_lpBMIH,
                                       CBM_INIT, m_lpImage, (LPBITMAPINFO) m_lpBMIH, DIB_RGB_COLORS);
    ASSERT(hBitmap != NULL);
    return hBitmap;
}

BOOL CDib::Compress(CDC* pDC, BOOL bCompress /* = TRUE */)
{
    // 1. makes GDI bitmap from existing DIB
    // 2. makes a new DIB from GDI bitmap with compression
    // 3. cleans up the original DIB
    // 4. puts the new DIB in the object
    if ((m_lpBMIH->biBitCount != 4) && (m_lpBMIH->biBitCount != 8)) return FALSE;

    // compression supported only for 4 bpp and 8 bpp DIBs
    if (m_hBitmap) return FALSE; // can't compress a DIB Section!

    TRACE("Compress: original palette size = %d\n", m_nColorTableEntries);
    HDC hdc = pDC->GetSafeHdc();
    HPALETTE hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);
    HBITMAP hBitmap;  // temporary

    if ((hBitmap = CreateBitmap(pDC)) == NULL) return FALSE;

    int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
    LPBITMAPINFOHEADER lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
    memcpy(lpBMIH, m_lpBMIH, nSize);  // new header

    if (bCompress) {
        switch (lpBMIH->biBitCount) {
        case 4:
            lpBMIH->biCompression = BI_RLE4;
            break;

        case 8:
            lpBMIH->biCompression = BI_RLE8;
            break;

        default:
            ASSERT(FALSE);
        }

        // calls GetDIBits with null data pointer to get size of compressed DIB
        if (!::GetDIBits(pDC->GetSafeHdc(), hBitmap, 0, (UINT) lpBMIH->biHeight,
                         NULL, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS)) {
            AfxMessageBox("Unable to compress this DIB");
            // probably a problem with the color table
            ::DeleteObject(hBitmap);
            delete [] lpBMIH;
            ::SelectPalette(hdc, hOldPalette, FALSE);
            return FALSE;
        }

        if (lpBMIH->biSizeImage == 0) {
            AfxMessageBox("Driver can't do compression");
            ::DeleteObject(hBitmap);
            delete [] lpBMIH;
            ::SelectPalette(hdc, hOldPalette, FALSE);
            return FALSE;
        } else {
            m_dwSizeImage = lpBMIH->biSizeImage;
        }
    } else {
        lpBMIH->biCompression = BI_RGB; // decompress
        // figure the image size from the bitmap width and height
        DWORD dwBytes = ((DWORD) lpBMIH->biWidth * lpBMIH->biBitCount) / 32;

        if (((DWORD) lpBMIH->biWidth * lpBMIH->biBitCount) % 32) {
            dwBytes++;
        }

        dwBytes *= 4;
        m_dwSizeImage = dwBytes * lpBMIH->biHeight; // no compression
        lpBMIH->biSizeImage = m_dwSizeImage;
    }

    // second GetDIBits call to make DIB
    LPBYTE lpImage = (LPBYTE) new char[m_dwSizeImage];
    VERIFY(::GetDIBits(pDC->GetSafeHdc(), hBitmap, 0, (UINT) lpBMIH->biHeight,
                       lpImage, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS));
    TRACE("dib successfully created - height = %d\n", lpBMIH->biHeight);
    ::DeleteObject(hBitmap);
    Empty();
    m_nBmihAlloc = m_nImageAlloc = crtAlloc;
    m_lpBMIH = lpBMIH;
    m_lpImage = lpImage;
    ComputeMetrics();
    ComputePaletteSize(m_lpBMIH->biBitCount);
    MakePalette();
    ::SelectPalette(hdc, hOldPalette, FALSE);
    TRACE("Compress: new palette size = %d\n", m_nColorTableEntries);
    return TRUE;
}

BOOL CDib::Read(CFile* pFile)
{
    // 1. read file header to get size of info hdr + color table
    // 2. read info hdr (to get image size) and color table
    // 3. read image
    // can't use bfSize in file header
    Empty();
    int nCount, nSize;
    BITMAPFILEHEADER bmfh;

    try {
        nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));

        if (nCount != sizeof(BITMAPFILEHEADER)) {
            throw new CException;
        }

        if (bmfh.bfType != 0x4d42) {
            throw new CException;
        }

        nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
        m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
        m_nBmihAlloc = m_nImageAlloc = crtAlloc;
        nCount = pFile->Read(m_lpBMIH, nSize); // info hdr & color table
        ComputeMetrics();
        ComputePaletteSize(m_lpBMIH->biBitCount);
        MakePalette();
        m_lpImage = (LPBYTE) new char[m_dwSizeImage];
        nCount = pFile->Read(m_lpImage, m_dwSizeImage); // image only
    } catch (CException* pe) {
        AfxMessageBox("Read error");
        pe->Delete();
        return FALSE;
    }

    return TRUE;
}

BOOL CDib::ReadSection(CFile* pFile, CDC* pDC /* = NULL */)
{
    // new function reads BMP from disk and creates a DIB section
    //    allows modification of bitmaps from disk
    // 1. read file header to get size of info hdr + color table
    // 2. read info hdr (to get image size) and color table
    // 3. create DIB section based on header parms
    // 4. read image into memory that CreateDibSection allocates
    Empty();
    int nCount, nSize;
    BITMAPFILEHEADER bmfh;

    try {
        nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));

        if (nCount != sizeof(BITMAPFILEHEADER)) {
            throw new CException;
        }

        if (bmfh.bfType != 0x4d42) {
            throw new CException;
        }

        nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
        m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
        m_nBmihAlloc = crtAlloc;
        m_nImageAlloc = noAlloc;
        nCount = pFile->Read(m_lpBMIH, nSize); // info hdr & color table

        if (m_lpBMIH->biCompression != BI_RGB) {
            throw new CException;
        }

        ComputeMetrics();
        ComputePaletteSize(m_lpBMIH->biBitCount);
        MakePalette();
        UsePalette(pDC);
        m_hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO) m_lpBMIH,
                                       DIB_RGB_COLORS, (LPVOID*) &m_lpImage, NULL, 0);
        ASSERT(m_lpImage != NULL);
        nCount = pFile->Read(m_lpImage, m_dwSizeImage); // image only
    } catch (CException* pe) {
        AfxMessageBox("ReadSection error");
        pe->Delete();
        return FALSE;
    }

    return TRUE;
}

BOOL CDib::Write(CFile* pFile)
{
    BITMAPFILEHEADER bmfh;
    bmfh.bfType = 0x4d42;  // 'BM'
    int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
    bmfh.bfSize = 0;
//  bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + m_dwSizeImage;
    // meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
    bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                     sizeof(RGBQUAD) * m_nColorTableEntries;

    try {
        pFile->Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
        pFile->Write((LPVOID) m_lpBMIH,  nSizeHdr);
        pFile->Write((LPVOID) m_lpImage, m_dwSizeImage);
    } catch (CException* pe) {
        pe->Delete();
        AfxMessageBox("write error");
        return FALSE;
    }

    return TRUE;
}

void CDib::Serialize(CArchive& ar)
{
    DWORD dwPos;
    dwPos = ar.GetFile()->GetPosition();
    TRACE("CDib::Serialize -- pos = %d\n", dwPos);
    ar.Flush();
    dwPos = ar.GetFile()->GetPosition();
    TRACE("CDib::Serialize -- pos = %d\n", dwPos);

    if (ar.IsStoring()) {
        Write(ar.GetFile());
    } else {
        Read(ar.GetFile());
    }
}

// helper functions
void CDib::ComputePaletteSize(int nBitCount)
{
    if ((m_lpBMIH == NULL) || (m_lpBMIH->biClrUsed == 0)) {
        switch (nBitCount) {
        case 1:
            m_nColorTableEntries = 2;
            break;

        case 4:
            m_nColorTableEntries = 16;
            break;

        case 8:
            m_nColorTableEntries = 256;
            break;

        case 16:
        case 24:
        case 32:
            m_nColorTableEntries = 0;
            break;

        default:
            ASSERT(FALSE);
        }
    } else {
        m_nColorTableEntries = m_lpBMIH->biClrUsed;
    }

    ASSERT((m_nColorTableEntries >= 0) && (m_nColorTableEntries <= 256));
}

void CDib::ComputeMetrics()
{
    if (m_lpBMIH->biSize != sizeof(BITMAPINFOHEADER)) {
        TRACE("Not a valid Windows bitmap -- probably an OS/2 bitmap\n");
        throw new CException;
    }

    m_dwSizeImage = m_lpBMIH->biSizeImage;

    if (m_dwSizeImage == 0) {
        DWORD dwBytes = ((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) / 32;

        if (((DWORD) m_lpBMIH->biWidth * m_lpBMIH->biBitCount) % 32) {
            dwBytes++;
        }

        dwBytes *= 4;
        m_dwSizeImage = dwBytes * m_lpBMIH->biHeight; // no compression
    }

    m_lpvColorTable = (LPBYTE) m_lpBMIH + sizeof(BITMAPINFOHEADER);
}

void CDib::Empty()
{
    // this is supposed to clean up whatever is in the DIB
    DetachMapFile();

    if (m_nBmihAlloc == crtAlloc) {
        delete [] m_lpBMIH;
    } else if (m_nBmihAlloc == heapAlloc) {
        ::GlobalUnlock(m_hGlobal);
        ::GlobalFree(m_hGlobal);
    }

    if (m_nImageAlloc == crtAlloc) delete [] m_lpImage;

    if (m_hPalette != NULL) ::DeleteObject(m_hPalette);

    if (m_hBitmap != NULL) ::DeleteObject(m_hBitmap);

    m_nBmihAlloc = m_nImageAlloc = noAlloc;
    m_hGlobal = NULL;
    m_lpBMIH = NULL;
    m_lpImage = NULL;
    m_lpvColorTable = NULL;
    m_nColorTableEntries = 0;
    m_dwSizeImage = 0;
    m_lpvFile = NULL;
    m_hMap = NULL;
    m_hFile = NULL;
    m_hBitmap = NULL;
    m_hPalette = NULL;
}

void CDib::DetachMapFile()
{
    if (m_hFile == NULL) return;

    ::UnmapViewOfFile(m_lpvFile);
    ::CloseHandle(m_hMap);
    ::CloseHandle(m_hFile);
    m_hFile = NULL;
}
