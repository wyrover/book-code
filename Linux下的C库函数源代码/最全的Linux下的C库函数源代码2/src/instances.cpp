// instances -- force DLL instances for Microsoft
#define __FORCE_INSTANCE

#include <complex>
#include <fstream>
#include <ios>
#include <istream>
#include <limits>
#include <locale>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <xlocale>
#include <xlocmes>
#include <xlocmon>
#include <xlocnum>
#include <xloctime>
#include <xstring>

#pragma warning(disable:4439)	// C4439: function with a managed parameter must have a __clrcall calling convention

_STD_BEGIN

template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, char *);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, char&);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, signed char *);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, signed char&);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, unsigned char *);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, unsigned char&);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<wchar_t, char_traits<wchar_t> >&, wchar_t *);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<wchar_t, char_traits<wchar_t> >&, wchar_t&);

 #ifdef _NATIVE_WCHAR_T_DEFINED
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<unsigned short, char_traits<unsigned short> >&, unsigned short *);
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<unsigned short, char_traits<unsigned short> >&, unsigned short&);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, const char *);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, char);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, const signed char *);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, signed char);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, const unsigned char *);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, unsigned char);
template _MRTIMP2 basic_ostream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<wchar_t, char_traits<wchar_t> >&, const wchar_t *);
template _MRTIMP2 basic_ostream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<wchar_t, char_traits<wchar_t> >&, wchar_t);

 #ifdef _NATIVE_WCHAR_T_DEFINED
template _MRTIMP2 basic_ostream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<unsigned short, char_traits<unsigned short> >&, const unsigned short *);
template _MRTIMP2 basic_ostream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<unsigned short, char_traits<unsigned short> >&, unsigned short);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

template _MRTIMP2 basic_string<char, char_traits<char>, allocator<char> > __CLRCALL_OR_CDECL operator+(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 basic_string<char, char_traits<char>, allocator<char> > __CLRCALL_OR_CDECL operator+(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 basic_string<char, char_traits<char>, allocator<char> > __CLRCALL_OR_CDECL operator+(
        const char, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 basic_string<char, char_traits<char>, allocator<char> > __CLRCALL_OR_CDECL operator+(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 basic_string<char, char_traits<char>, allocator<char> > __CLRCALL_OR_CDECL operator+(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const basic_string<char, char_traits<char>, allocator<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const char *, const basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const basic_string<char, char_traits<char>, allocator<char> >&, const char *);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&,
        basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL getline(
        basic_istream<char, char_traits<char> >&,
        basic_string<char, char_traits<char>, allocator<char> >&);
template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL getline(
        basic_istream<char, char_traits<char> >&,
        basic_string<char, char_traits<char>, allocator<char> >&, const char);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&,
        const basic_string<char, char_traits<char>, allocator<char> >&);

template _MRTIMP2 basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > __CLRCALL_OR_CDECL operator+(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > __CLRCALL_OR_CDECL operator+(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > __CLRCALL_OR_CDECL operator+(
        const wchar_t, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > __CLRCALL_OR_CDECL operator+(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > __CLRCALL_OR_CDECL operator+(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const wchar_t *, const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t *);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<wchar_t, char_traits<wchar_t> >&,
        basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL getline(
        basic_istream<wchar_t, char_traits<wchar_t> >&,
        basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL getline(
        basic_istream<wchar_t, char_traits<wchar_t> >&,
        basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&, const wchar_t);
template _MRTIMP2 basic_ostream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<wchar_t, char_traits<wchar_t> >&,
        const basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >&);

 #ifdef _NATIVE_WCHAR_T_DEFINED
template _MRTIMP2 basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> > __CLRCALL_OR_CDECL operator+(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> > __CLRCALL_OR_CDECL operator+(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> > __CLRCALL_OR_CDECL operator+(
        const unsigned short, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> > __CLRCALL_OR_CDECL operator+(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> > __CLRCALL_OR_CDECL operator+(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator<=(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const unsigned short *, const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator>=(
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short *);
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<unsigned short, char_traits<unsigned short> >&,
        basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL getline(
        basic_istream<unsigned short, char_traits<unsigned short> >&,
        basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL getline(
        basic_istream<unsigned short, char_traits<unsigned short> >&,
        basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&, const unsigned short);
template _MRTIMP2 basic_ostream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<unsigned short, char_traits<unsigned short> >&,
        const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, complex<float>&);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, const complex<float>&);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<wchar_t, char_traits<wchar_t> >&, complex<float>&);
template _MRTIMP2 basic_ostream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<wchar_t, char_traits<wchar_t> >&, const complex<float>&);

 #ifdef _NATIVE_WCHAR_T_DEFINED
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<unsigned short, char_traits<unsigned short> >&, complex<float>&);
template _MRTIMP2 basic_ostream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<unsigned short, char_traits<unsigned short> >&, const complex<float>&);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, complex<double>&);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, const complex<double>&);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<wchar_t, char_traits<wchar_t> >&, complex<double>&);
template _MRTIMP2 basic_ostream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<wchar_t, char_traits<wchar_t> >&, const complex<double>&);

 #ifdef _NATIVE_WCHAR_T_DEFINED
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<unsigned short, char_traits<unsigned short> >&, complex<double>&);
template _MRTIMP2 basic_ostream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<unsigned short, char_traits<unsigned short> >&, const complex<double>&);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

template _MRTIMP2 basic_istream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<char, char_traits<char> >&, complex<long double>&);
template _MRTIMP2 basic_ostream<char, char_traits<char> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<char, char_traits<char> >&, const complex<long double>&);
template _MRTIMP2 basic_istream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<wchar_t, char_traits<wchar_t> >&, complex<long double>&);
template _MRTIMP2 basic_ostream<wchar_t, char_traits<wchar_t> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<wchar_t, char_traits<wchar_t> >&, const complex<long double>&);

 #ifdef _NATIVE_WCHAR_T_DEFINED
template _MRTIMP2 basic_istream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator>>(
        basic_istream<unsigned short, char_traits<unsigned short> >&, complex<long double>&);
template _MRTIMP2 basic_ostream<unsigned short, char_traits<unsigned short> >& __CLRCALL_OR_CDECL operator<<(
        basic_ostream<unsigned short, char_traits<unsigned short> >&, const complex<long double>&);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

template _MRTIMP2 float __CLRCALL_OR_CDECL imag(const complex<float>&);
template _MRTIMP2 float __CLRCALL_OR_CDECL real(const complex<float>&);
template _MRTIMP2 float __CLRCALL_OR_CDECL _Fabs(const complex<float>&, int *);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator+(const complex<float>&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator+(const complex<float>&, const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator+(const float&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator-(const complex<float>&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator-(const complex<float>&, const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator-(const float&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator*(const complex<float>&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator*(const complex<float>&, const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator*(const float&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator/(const complex<float>&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator/(const complex<float>&, const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator/(const float&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator+(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL operator-(const complex<float>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const complex<float>&, const complex<float>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const complex<float>&, const float&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const float&, const complex<float>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const complex<float>&, const complex<float>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const complex<float>&, const float&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const float&, const complex<float>&);
template _MRTIMP2 float __CLRCALL_OR_CDECL abs(const complex<float>&);
template _MRTIMP2 float __CLRCALL_OR_CDECL arg(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL conj(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL cos(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL cosh(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL exp(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL log(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL log10(const complex<float>&);
template _MRTIMP2 float __CLRCALL_OR_CDECL norm(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL polar(const float&, const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL polar(const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL pow(const complex<float>&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL pow(const complex<float>&, const float&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL pow(const complex<float>&, int);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL pow(const float&, const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL sin(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL sinh(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL sqrt(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL tanh(const complex<float>&);
template _MRTIMP2 complex<float> __CLRCALL_OR_CDECL tan(const complex<float>&);

template _MRTIMP2 double __CLRCALL_OR_CDECL imag(const complex<double>&);
template _MRTIMP2 double __CLRCALL_OR_CDECL real(const complex<double>&);
template _MRTIMP2 double __CLRCALL_OR_CDECL _Fabs(const complex<double>&, int *);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator+(const complex<double>&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator+(const complex<double>&, const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator+(const double&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator-(const complex<double>&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator-(const complex<double>&, const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator-(const double&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator*(const complex<double>&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator*(const complex<double>&, const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator*(const double&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator/(const complex<double>&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator/(const complex<double>&, const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator/(const double&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator+(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL operator-(const complex<double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const complex<double>&, const complex<double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const complex<double>&, const double&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const double&, const complex<double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const complex<double>&, const complex<double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const complex<double>&, const double&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const double&, const complex<double>&);
template _MRTIMP2 double __CLRCALL_OR_CDECL abs(const complex<double>&);
template _MRTIMP2 double __CLRCALL_OR_CDECL arg(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL conj(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL cos(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL cosh(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL exp(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL log(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL log10(const complex<double>&);
template _MRTIMP2 double __CLRCALL_OR_CDECL norm(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL polar(const double&, const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL polar(const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL pow(const complex<double>&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL pow(const complex<double>&, const double&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL pow(const complex<double>&, int);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL pow(const double&, const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL sin(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL sinh(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL sqrt(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL tanh(const complex<double>&);
template _MRTIMP2 complex<double> __CLRCALL_OR_CDECL tan(const complex<double>&);

template _MRTIMP2 long double __CLRCALL_OR_CDECL imag(const complex<long double>&);
template _MRTIMP2 long double __CLRCALL_OR_CDECL real(const complex<long double>&);
template _MRTIMP2 long double __CLRCALL_OR_CDECL _Fabs(const complex<long double>&, int *);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator+(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator+(const complex<long double>&, const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator+(const long double&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator-(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator-(const complex<long double>&, const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator-(const long double&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator*(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator*(const complex<long double>&, const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator*(const long double&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator/(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator/(const complex<long double>&, const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator/(const long double&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator+(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL operator-(const complex<long double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const complex<long double>&, const long double&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator==(const long double&, const complex<long double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const complex<long double>&, const long double&);
template _MRTIMP2 bool __CLRCALL_OR_CDECL operator!=(const long double&, const complex<long double>&);
template _MRTIMP2 long double __CLRCALL_OR_CDECL abs(const complex<long double>&);
template _MRTIMP2 long double __CLRCALL_OR_CDECL arg(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL conj(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL cos(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL cosh(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL exp(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL log(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL log10(const complex<long double>&);
template _MRTIMP2 long double __CLRCALL_OR_CDECL norm(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL polar(const long double&, const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL polar(const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL pow(const complex<long double>&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL pow(const complex<long double>&, const long double&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL pow(const complex<long double>&, int);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL pow(const long double&, const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL sin(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL sinh(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL sqrt(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL tanh(const complex<long double>&);
template _MRTIMP2 complex<long double> __CLRCALL_OR_CDECL tan(const complex<long double>&);

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
