#ifndef __CDIB_H
#define __CDIB_H

class CDib : public CObject
{
public:
    RGBQUAD* m_pRGB;
    BYTE* m_pData;
    UINT m_numberOfColors;
	BOOL m_valid;
    BITMAPFILEHEADER bitmapFileHeader;

    BITMAPINFOHEADER* m_pBitmapInfoHeader;
    BITMAPINFO* m_pBitmapInfo;
    BYTE* pDib;
	DWORD size;
    int byBitCount;
    DWORD dwWidthBytes;
public:
    CDib();
    ~CDib();

	char m_fileName[256];
    char* GetFileName();
    BOOL IsValid();
    DWORD GetSize();
    UINT GetWidth();
    UINT GetHeight();
    UINT GetNumberOfColors();
    RGBQUAD* GetRGB();
    BYTE* GetData();
    BITMAPINFO* GetInfo();
	DWORD GetDibWidthBytes();

	WORD PaletteSize(LPBYTE lpDIB);
	WORD DIBNumColors(LPBYTE lpDIB);
    void SaveFile(const CString filename);

public:
    void LoadFile(const char* dibFileName);
	
};

#endif

