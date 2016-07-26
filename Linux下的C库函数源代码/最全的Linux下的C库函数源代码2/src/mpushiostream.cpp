#ifdef MRTDLL
#undef MRTDLL
#endif

// This file is built into msvcurt.lib
#if !defined(_M_CEE_PURE)
#error This file must be built with /clr:pure.
#endif

 #ifdef _NATIVE_WCHAR_T_DEFINED
  #include <fstream>

  #ifndef wistream

  #define wistream    ushistream
  #define wostream    ushostream
  #define wfilebuf    ushfilebuf
  #define _Init_wcerr _Init_ushcerr
  #define _Init_wcout _Init_ushcout
  #define _Init_wclog _Init_ushclog
  #define _Init_wcin  _Init_ushcin
  #define _Winit      _UShinit
  #define init_wcerr  init_ushcerr
  #define init_wcout  init_ushcout
  #define init_wclog  init_ushclog
  #define init_wcin   init_ushcin 
  #endif

  #include <iostream>

namespace std
{
  extern _MRTIMP2 wistream& __cdecl _Wcin_func();
  extern _MRTIMP2 wostream& __cdecl _Wcout_func();
  extern _MRTIMP2 wostream& __cdecl _Wcerr_func();
  extern _MRTIMP2 wostream& __cdecl _Wclog_func();
}

#pragma warning(push)
#pragma warning(disable:4074) // warning C4074: initializers put in compiler reserved initialization area
#pragma init_seg(compiler)
#pragma warning(pop)

std::ushistream &std::wcin  = std::_Wcin_func();
std::ushostream &std::wcout = std::_Wcout_func();
std::ushostream &std::wclog = std::_Wclog_func();
std::ushostream &std::wcerr = std::_Wcerr_func();

 #endif
#include <fstream>

