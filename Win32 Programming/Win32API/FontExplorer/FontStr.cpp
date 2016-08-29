#include "stdafx.h"
#include "fontstr.h"

/****************************************************************************
*                                FontToString
* Inputs:
*       LPSTR buffer: Place to write formatted font string
*       LOGFONT * lf: Logical font to convert to external form
*       CDC * dc: DC for display (used to convert height properly)
*       BOOL quotes: TRUE to put quotes around the font name, FALSE to omit
*                    quotes
* Result: void
*       
* Effect: 
*       Converts the logical font to a string that can be processed by
*       StringToFont (if quotes is TRUE) or used for display (if quotes
*       is FALSE)
****************************************************************************/

void FontToString(LPSTR buffer, LOGFONT * lf, CDC * dc, BOOL quotes)
    {
     char style[50];
     int height;
     int factor;

     if(lf->lfWeight == FW_EXTRABOLD && !lf->lfItalic)
        strcpy(style,"ExtraBold");
     else
     if(lf->lfWeight == FW_BOLD && !lf->lfItalic)
        strcpy(style,"Bold");
     else
     if(lf->lfWeight == FW_BOLD && lf->lfItalic)
        strcpy(style,"BoldItalic");
     else
     if(lf->lfWeight == FW_NORMAL && lf->lfItalic)
        strcpy(style,"Italic");
     else
     if(lf->lfWeight == FW_NORMAL && !lf->lfItalic)
        strcpy(style,"Normal");
     else
        wsprintf(style,"Weight %d", lf->lfWeight);
     
     factor = dc->GetDeviceCaps(LOGPIXELSX);
     height = -MulDiv(lf->lfHeight, 72, factor);
     wsprintf(buffer,"%s%s%s %d %s",
                (quotes ? "\"" : ""), 
                lf->lfFaceName, 
                (quotes ? "\"" : ""), 
                height, style);
    }
