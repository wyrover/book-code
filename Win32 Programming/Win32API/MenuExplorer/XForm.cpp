#include "stdafx.h"
#include "xform.h"

/****************************************************************************
*                                charToNybble
* Inputs:
*       TCHAR t: character to convert
* Result: int
*       nybble value, or 0 if not valid hex digit
* Effect: 
*       Converts the character to its hex nybble code
****************************************************************************/

static int charToNybble(TCHAR t)
    {
     switch(t)
        { /* decode */
	 case _T('0'):
	 case _T('1'):
	 case _T('2'):
	 case _T('3'):
	 case _T('4'):
	 case _T('5'):
	 case _T('6'):
	 case _T('7'):
	 case _T('8'):
	 case _T('9'):
		 return t - _T('0');
	 case _T('a'):
	 case _T('b'):
	 case _T('c'):
	 case _T('d'):
	 case _T('e'):
	 case _T('f'):
		 return t - _T('a') + 10;
	 case _T('A'):
	 case _T('B'):
	 case _T('C'):
	 case _T('D'):
	 case _T('E'):
	 case _T('F'):
		 return t - _T('A') + 10;
	} /* decode */
     return 0;
    }

/****************************************************************************
*                                  escToStr
* Inputs:
*       CString s: String which contains escape characters
* Result: CString
*       New CString (temporary object) which has translated string
* Effect: 
*       Creates a string which is the result of processing the escape 
*	characters in string s.
* Notes:
*	We don't process the complete set, just the following:
*		\t - tab
*		\\ - \
*		\f - formfeed
*		\r - return
*		\n - newline
*		\xdd - hex character
****************************************************************************/

CString escToStr(CString s)
    {
     CString t;
     int len = s.GetLength();

     for(int i = 0; i < len; i++)
        { /* copy */
	 if(s[i] == _T('\\'))
	    { /* escape */
	     i++;
	     switch(s[i])
	        { /* decode */
		 case _T('x'):
			 t += (TCHAR) (charToNybble(s[i]) * 16 + 
			 	       charToNybble(s[i+1]));
			 i += 2;
			 break;
		 case _T('\\'):
			 t += _T('\\');
			 break;
		 case _T('n'):
			 t += _T('\n');
			 break;
		 case _T('t'):
			 t += _T('\t');
			 break;
		 case _T('r'):
			 t += _T('\r');
			 break;
		} /* decode */
	    } /* escape */
	 else
	    t += s[i];
	} /* copy */
     return t;
     
    }

/****************************************************************************
*				   charToEsc
* Inputs:
*       TCHAR ch: Character to convert
* Result: CString
*       
* Effect: 
*       A printable representation of the character
****************************************************************************/

CString charToEsc(TCHAR ch)
    {
     CString chstr;

     switch(ch)
        { /* special cases */
	 case _T('\0'):
		 chstr = _T("\\0");
		 break;
	 case _T('\1'):
		 chstr = _T("\\1");
		 break;
	 case _T('\a'):
		 chstr = _T("\\a");
		 break;
	 case _T('\b'):
		 chstr = _T("\\b");
		 break;
	 case _T('\f'):
		 chstr = _T("\\f");
		 break;
	 case _T('\n'):
		 chstr = _T("\\n");
		 break;
	 case _T('\r'):
		 chstr = _T("\\r");
		 break;
	 case _T('\t'):
		 chstr = _T("\\t");
		 break;
	 case _T('\v'):
		 chstr = _T("\\v");
		 break;
	 default:
		 chstr.Format(_T("%c"), ch);
		 break;
	} /* special cases */
     
     return chstr;
    }

/****************************************************************************
*                                  strToEsc
* Inputs:
*       CString s: String to be expanded
* Result: CString
*       String with all characters expanded
* Effect: 
*       Creates a new string which is the character-by-character expansion
*	of the input string
* Note:
*	This expands only the control characters (codes 0..31) and leaves all
*	other characters intact
****************************************************************************/

CString strToEsc(CString s)
    {
     CString newstr;
     for(int i = 0; i < s.GetLength(); i++)
        { /* expand loop */
	 if(_T('\0') <= s[i] && s[i] < _T(' '))
	    newstr += charToEsc(s[i]);
         else
	    newstr += s[i];
	} /* expand loop */
     return newstr;
    }
