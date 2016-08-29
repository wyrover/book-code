#include "stdafx.h"

/****************************************************************************
*                                getBitmapSize
* Inputs:
*       CDC * dc: Display context.  
*       HBITMAP: Bitmap handle
* Result: CSize
*       Bitmap dimensions
****************************************************************************/

CSize getBitmapSize(CDC * dc, HBITMAP bitmap)
    {
     BITMAPINFO bmi;
     CSize sz;

     bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
     bmi.bmiHeader.biBitCount = 0;  // want size returned
     if(::GetDIBits(dc->m_hDC, bitmap, 0, 0, NULL, &bmi, DIB_RGB_COLORS) == 0)
        { /* failed */
         sz.cx = 0;
         sz.cy = 0;
         return sz;
        } /* failed */
     else
        { /* success */
         sz.cx = bmi.bmiHeader.biWidth;
         sz.cy = bmi.bmiHeader.biHeight;
         return sz;
        } /* success */
    }

/****************************************************************************
*                              BOOL isMonochrome
* Inputs:
*       HBITMAP bm: Bitmap
* Result: BOOL
*       TRUE if bitmap is monochrome
*       FALSE if bitmap is not monochrome
* Effect: 
*       Will temporarily 
****************************************************************************/

BOOL isMonochrome(CDC * dc, HBITMAP bm)
    {
     BITMAPINFO bmi;

     bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
     bmi.bmiHeader.biBitCount = 0;  // want size returned
     ::GetDIBits(dc->m_hDC, bm, 0, 0, NULL, &bmi, DIB_RGB_COLORS);
     return (bmi.bmiHeader.biBitCount == 1);
     
    }
