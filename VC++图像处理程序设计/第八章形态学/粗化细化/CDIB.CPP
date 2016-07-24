#include "stdafx.h"
#include "cdib.h"
#include "windowsx.h"
#include "math.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

CDib::CDib()
{
//	LoadFile();
}

CDib::~CDib()
{
    GlobalFreePtr(m_pBitmapInfo);
}

void CDib::LoadFile(const char* dibFileName)
{
	strcpy(m_fileName,dibFileName);
    CFile dibFile(m_fileName, CFile::modeRead);
    
    dibFile.Read((void*)&bitmapFileHeader,sizeof(BITMAPFILEHEADER));

    if (bitmapFileHeader.bfType == 0x4d42)
    {
        DWORD fileLength = dibFile.GetLength();    
        DWORD size = fileLength -
			sizeof(BITMAPFILEHEADER);
        BYTE* pDib =
            (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, size);
        dibFile.Read((void*)pDib, size);
        dibFile.Close();

        m_pBitmapInfo = (BITMAPINFO*) pDib;
        m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pDib;
        m_pRGB = (RGBQUAD*)(pDib +
			m_pBitmapInfoHeader->biSize);
        int m_numberOfColors = GetNumberOfColors();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed =
			    m_numberOfColors;
        DWORD colorTableSize = m_numberOfColors *
            sizeof(RGBQUAD);
        m_pData = pDib + m_pBitmapInfoHeader->biSize
            + colorTableSize;
		if (m_pRGB == (RGBQUAD*)m_pData) // No color table
			m_pRGB = NULL;
        m_pBitmapInfoHeader->biSizeImage = GetSize();
		m_valid = TRUE;
    }    
    else
    {
        m_valid = FALSE;
        AfxMessageBox("This isn't a bitmap file!");
    }
}

BOOL CDib::IsValid()
{
    return m_valid;
}
        
char* CDib::GetFileName()
{
    return m_fileName;
}
        
UINT CDib::GetWidth()
{
    return (UINT) m_pBitmapInfoHeader->biWidth;
}
        
UINT CDib::GetHeight()
{
    return (UINT) m_pBitmapInfoHeader->biHeight;
}
        
DWORD CDib::GetSize()
{
    if (m_pBitmapInfoHeader->biSizeImage != 0)
        return m_pBitmapInfoHeader->biSizeImage;
	else
    {
        DWORD height = (DWORD) GetHeight();
        DWORD width = (DWORD) GetWidth();
        return height * width;
    }
}

UINT CDib::GetNumberOfColors()
{
	int numberOfColors;

    if ((m_pBitmapInfoHeader->biClrUsed == 0) &&
          (m_pBitmapInfoHeader->biBitCount < 9))
	{
		switch (m_pBitmapInfoHeader->biBitCount)
		{
		    case 1: numberOfColors = 2; break;
		    case 4: numberOfColors = 16; break;
		    case 8: numberOfColors = 256;
		}
	}
    else
		numberOfColors = (int) m_pBitmapInfoHeader->biClrUsed;

    return numberOfColors;
}
    
BYTE* CDib::GetData()
{
    return m_pData;
}

RGBQUAD* CDib::GetRGB()
{
    return m_pRGB;
}

BITMAPINFO* CDib::GetInfo()
{
    return m_pBitmapInfo;
}

WORD CDib::PaletteSize(LPBYTE lpDIB) 
{ 
    return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
} 

WORD CDib::DIBNumColors(LPBYTE lpDIB) 
{ 
    WORD wBitCount;  // DIB bit count 
    wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
    switch (wBitCount) 
    { 
        case 1: 
            return 2; 
        case 4: 
            return 16; 
        case 8: 
            return 256; 
        default: 
            return 0; 
    } 
} 

void CDib::SaveFile(const CString filename)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file 
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
    DWORD               dwDIBSize; 
 
    bmfHdr.bfType = 0x4d42;  // "BM" 
    lpBI = (LPBITMAPINFOHEADER)m_pBitmapInfoHeader; 
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only 
        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
        dwDIBSize += dwBmBitsSize; 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
	CFile dibFile(filename, CFile::modeWrite|CFile::modeCreate);
	dibFile.Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
	dibFile.WriteHuge(lpBI, dwDIBSize);
	dibFile.Close();
/*
bitmapFileHeader.bfSize=GetSize()+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO);
bitmapFileHeader.bfType = 0x4d42;
bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO);
bitmapFileHeader.bfReserved1 = 0;
bitmapFileHeader.bfReserved2 = 0;
CFile dibFile(filename, CFile::modeWrite|CFile::modeCreate);
dibFile.Write(&bitmapFileHeader,sizeof(BITMAPFILEHEADER));
dibFile.Write(m_pBitmapInfo,sizeof(BITMAPINFO));
dibFile.WriteHuge(m_pData,GetSize());
dibFile.Close();
}
//////////////////////////////////////////////////////////////////
    BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int sizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorEntries;
	bmfh.bfSize = 0;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorEntries;	
	try {
		pFile->Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		pFile->Write((LPVOID) m_lpBMPHdr,  sizeHdr);
		pFile->Write((LPVOID) m_lpDIBits, m_dwImageSize);
	}
	catch(CException* pe) {
		pe->Delete();
		AfxMessageBox("write error");
		return FALSE;
	}
	return TRUE;
	}
	/////////////////////////////////////////////////
	WaitCursorBegin();

    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file 
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
    DWORD               dwDIBSize; 

	// Get a pointer to the DIB memory, the first of which contains 
    // a BITMAPINFO structure 
    lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
		WaitCursorEnd();
        return FALSE; 
	}
 
    // Check to see if we're dealing with an OS/2 DIB.  If so, don't 
    // save it because our functions aren't written to deal with these 
    // DIBs. 
    if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
    { 
        GlobalUnlock(m_hDib); 
		WaitCursorEnd();
        return FALSE; 
    } 
 
    // Fill in the fields of the file header 
 
    // Fill in file type (first 2 bytes must be "BM" for a bitmap) 
 
    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM" 
 
    // Calculating the size of the DIB is a bit tricky (if we want to 
    // do it right).  The easiest way to do this is to call GlobalSize() 
    // on our global handle, but since the size of our global memory may have 
    // been padded a few bytes, we may end up writing out a few too 
    // many bytes to the file (which may cause problems with some apps, 
    // like HC 3.0). 
    // 
    // So, instead let's calculate the size manually. 
    // 
    // To do this, find size of header plus size of color table.  Since the 
    // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains 
    // the size of the structure, let's use this. 
 
    // Partial Calculation 
 
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
 
    // Now calculate the size of the image 
 
    // It's an RLE bitmap, we can't calculate size, so trust the biSizeImage 
    // field 
 
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only 
 
        // It's not RLE, so size is Width (DWORD aligned) * Height 
 
        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
 
        dwDIBSize += dwBmBitsSize; 
 
        // Now, since we have calculated the correct size, why don't we 
        // fill in the biSizeImage field (this will fix any .BMP files which  
        // have this field incorrect). 
 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
 
 
    // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER) 
                    
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
 
    // Now, calculate the offset the actual bitmap bits will be in 
    // the file -- It's the Bitmap file header plus the DIB header, 
    // plus the size of the color table. 
     
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
 	TRY
	{
	    // Write the file header 
		pFile->Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
		// write DIB buffer
		pFile->WriteHuge(lpBI, dwDIBSize);
	}
	CATCH (CException, e)
	{
        GlobalUnlock(m_hDib); 
		WaitCursorEnd();
		return FALSE;
	}
	END_CATCH

	GlobalUnlock(m_hDib); 
	WaitCursorEnd();
	
	return TRUE;
*/
}

DWORD CDib::GetDibWidthBytes()
{
	byBitCount=m_pBitmapInfoHeader->biBitCount;
	LONG nWidth=m_pBitmapInfoHeader->biWidth;

	dwWidthBytes = (DWORD)m_pBitmapInfoHeader->biWidth;	//8-bits
	if(byBitCount == 1) dwWidthBytes = (nWidth + 7) / 8;
	else if(byBitCount == 4) dwWidthBytes = (nWidth + 1) / 2;
	else if(byBitCount == 24) dwWidthBytes = 3 * nWidth ;
	
	while((dwWidthBytes & 3) != 0)dwWidthBytes++;

	return dwWidthBytes;
}
