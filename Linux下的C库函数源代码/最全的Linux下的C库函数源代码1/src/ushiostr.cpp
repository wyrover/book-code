// ushiostream -- _UShinit members, dummy for Microsoft
// unsigned short version for wiostram

 #ifdef _NATIVE_WCHAR_T_DEFINED
  #include <fstream>

  #define wistream    ushistream
  #define wostream    ushostream
  #define wfilebuf    ushfilebuf
  #define _Init_wcerr _Init_ushcerr
  #define _Init_wcout _Init_ushcout
  #define _Init_wclog _Init_ushclog
  #define _Init_wcin  _Init_ushcin
  #define _Winit      _UShinit

  #include <iostream>

_STD_BEGIN
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wistream *_Ptr_wcin = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream *_Ptr_wcout = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream *_Ptr_wcerr = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream *_Ptr_wclog = 0;
_STD_END

  #include "wiostrea.cpp"
 #endif

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
