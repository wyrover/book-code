
#include "stdafx.h"
#include "bitmap.h"

/****************************************************************************
*                          CBitmapData::~CBitmapData
* Effect: 
*       Destructor
****************************************************************************/

CBitmapData::~CBitmapData()
    {
     delete [](LPBYTE)pbmi;
     delete []bits;
    }

/****************************************************************************
*                          CBitmapData::CBitmapData
* Inputs:
*       HBITMAP hbmap: Bitmap handle
* Effect: 
*       Allocates space for the bitmap and for the BITMAPINFOHEADER with
*	full color information.
* Exceptions:
*	AfxThrowMemoryException if allocation failure
* Notes:
*	Initializes the pbmi field in the structure
*
*			   |&pbmi->bmiColors[0]
*   pbmi:                  |<---ps---->|
*	+------------------+-----------+
*	| BITMAPINFOHEADER | RGB quads |
*	+------------------+-----------+
*       		ps = (colors used) * sizeof(RGBQUAD)
*
* Notes:
*	We first find out some basic information about the bitmap, such
*	as its size, then we allocate a structure big enough to hold the
*	BITMAPINFOHEADER plus the necessary RGB quads, then we finally
*	realloc the structure to be big enough to hold the entire bitmap
*	and append the bitmap to the end.
****************************************************************************/

CBitmapData::CBitmapData(HBITMAP hbmap)
    {
     BITMAP bm;
     BITMAPINFO bmi;  // bitmap info to get basic factoids
     CClientDC dc(NULL);

     hbm  = hbmap;
     pbmi = NULL;
     bits = NULL;

     if(hbm == NULL)
	return;

     // Get basic bitmap properties
     ::GetObject(hbm, sizeof(bm), (LPVOID)&bm);

     int bitcount = bm.bmPlanes * bm.bmBitsPixel;


     bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
     bmi.bmiHeader.biWidth = bm.bmWidth;
     bmi.bmiHeader.biHeight = bm.bmHeight;
     bmi.bmiHeader.biPlanes = 0;
     bmi.bmiHeader.biBitCount = 0;
     bmi.bmiHeader.biSizeImage = 0;
     bmi.bmiHeader.biXPelsPerMeter = 0;
     bmi.bmiHeader.biYPelsPerMeter = 0;
     bmi.bmiHeader.biClrUsed = 0;
     bmi.bmiHeader.biClrImportant = 0;

     ::GetDIBits(dc.m_hDC, hbm, 0, 0, NULL, &bmi, DIB_RGB_COLORS);

     int numcolors = bmi.bmiHeader.biClrUsed;
     if(numcolors == 0)
        { /* synthesize it */
	 switch(bmi.bmiHeader.biBitCount)
	    { /* biBitCount */
	     case 1: numcolors = 2; break;
	     case 4: numcolors = 16; break;
	     case 8: numcolors = 256; break;
	     default: numcolors = 0; break;
	    } /* biBitCount */
	} /* synthesize it */

     if(numcolors == 0)
	return;  // uninitialized

     // We may have to synthesize the biSizeImage field; see the SHOWDIB
     // same on MSDN

#define WIDTHBYTES(i)   ((i+31)/32*4)
     if(bmi.bmiHeader.biSizeImage == 0)
	bmi.bmiHeader.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * bitcount);
     
     int palettesize = numcolors * sizeof(RGBQUAD);
     int len = bmi.bmiHeader.biSize + palettesize;

     pbmi = (LPBITMAPINFO) new BYTE[len];
     if(pbmi == NULL)
	AfxThrowMemoryException();

     memset((LPBYTE)pbmi, 0, len);

     pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);

     ::GetDIBits(dc.m_hDC, hbm, 0, 0, NULL,
     			pbmi, (DWORD)DIB_RGB_COLORS);

     if(pbmi->bmiHeader.biSizeImage == 0)
        { /* make one up */
	 
	 pbmi->bmiHeader.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * bitcount) 
	 						     * bm.bmHeight;
	} /* make one up */

     bits = (LPVOID)new BYTE[pbmi->bmiHeader.biSizeImage];
     if(bits == NULL)
	AfxThrowMemoryException();

     ::GetDIBits(dc.m_hDC, hbm, 0, (DWORD)bmi.bmiHeader.biHeight,
     			(LPBYTE)bits, pbmi, DIB_RGB_COLORS);
    }

/****************************************************************************
*                                 operator==
* Inputs:
*       CBitmapData & b1:
*	CBitmapData & b2:
* Result: BOOL
*       TRUE if bitmap data is equal
*	FALSE if bitmap data is unequal
****************************************************************************/

BOOL operator==(const CBitmapData & b1, const CBitmapData & b2)
    {
     UINT len1;
     len1 = b1.pbmi->bmiHeader.biSizeImage;
     if(len1 != b2.pbmi->bmiHeader.biSizeImage)
	return FALSE;

     if(memcmp(b1.bits, b2.bits, len1) == 0)
	return TRUE;

     return FALSE;
    }
