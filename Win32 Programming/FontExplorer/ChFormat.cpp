#include "stdafx.h"

/****************************************************************************
*                          CFontExplorer::charFormat
* Inputs:
*       CString& s: String reference
*       TCHAR ch: Character to format
* Result: void
*       
* Effect: 
*       Formats the string in hex, decimal and character displays
****************************************************************************/

void charFormat(CString & s, TCHAR ch)
    {
     s.Format(_T("0x%02x %d \"%c\""), ch, ch, ch);
    }
