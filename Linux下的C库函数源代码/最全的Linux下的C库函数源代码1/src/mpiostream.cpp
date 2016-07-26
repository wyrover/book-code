#ifdef MRTDLL
#undef MRTDLL
#endif

// This file is built into msvcurt.lib
#if !defined(_M_CEE_PURE)
#error This file must be built with /clr:pure.
#endif

#include <iostream>
#include <xiosbase>
#include <xlocale>

namespace std
{
extern _MRTIMP2 istream& __cdecl _Cin_func();
extern _MRTIMP2 ostream& __cdecl _Cout_func();
extern _MRTIMP2 ostream& __cdecl _Cerr_func();
extern _MRTIMP2 ostream& __cdecl _Clog_func();

extern _MRTIMP2 wistream& __cdecl _Wcin_func();
extern _MRTIMP2 wostream& __cdecl _Wcout_func();
extern _MRTIMP2 wostream& __cdecl _Wcerr_func();
extern _MRTIMP2 wostream& __cdecl _Wclog_func();

extern _MRTIMP2 const streamoff& __cdecl _BADOFF_func();
extern _MRTIMP2 fpos_t& __cdecl _Fpz_func();
}

#pragma warning(push)
#pragma warning(disable:4074) // warning C4074: initializers put in compiler reserved initialization area
#pragma init_seg(compiler)
#pragma warning(pop)

namespace std
{
    // Definitions
extern istream& cin  = _Cin_func();
extern ostream& cout = _Cout_func();
extern ostream& clog = _Clog_func();
extern ostream& cerr = _Cerr_func();

extern wistream& wcin  = _Wcin_func();
extern wostream& wcout = _Wcout_func();
extern wostream& wclog = _Wclog_func();
extern wostream& wcerr = _Wcerr_func();

extern const streamoff& _BADOFF = _BADOFF_func();
extern fpos_t& _Fpz = _Fpz_func();
}

int& std::locale::id::_Id_cnt =
    std::locale::id::_Id_cnt_func();
std::locale::id& std::ctype<char>::id =
    std::ctype<char>::_Id_func();
std::locale::id& std::ctype<wchar_t>::id =
    std::ctype<wchar_t>::_Id_func();
std::locale::id& std::ctype<unsigned short>::id =
    std::ctype<unsigned short>::_Id_func();
std::locale::id& std::codecvt<wchar_t, char, mbstate_t>::id =
    std::codecvt<wchar_t, char, mbstate_t>::_Id_func();
std::locale::id& std::codecvt<unsigned short, char, mbstate_t>::id =
    std::codecvt<unsigned short, char, mbstate_t>::_Id_func();
int &std::ios_base::Init::_Init_cnt =
    std::ios_base::Init::_Init_cnt_func();
int& std::ios_base::_Index =
    std::ios_base::_Index_func();
bool& std::ios_base::_Sync =
    std::ios_base::_Sync_func();
std::locale::_Locimp *&std::locale::_Locimp::_Clocptr =
    std::locale::_Locimp::_Clocptr_func();

