/* _Getwctype -- return character classification flags for wide character */
#include <xlocinfo.h>
#include <wchar.h>
#include <awint.h>
#include <yvals.h>

_C_STD_BEGIN
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _Getwctype(wchar_t _Ch,
	const _Ctypevec *_Ctype)
	{	/* return character classification flags for _Ch */
	short _Mask;
	return ((short)(__crtGetStringTypeW(NULL, CT_CTYPE1, &_Ch, 1,
		(LPWORD)&_Mask, _Ctype->_Page, _Ctype->_Hand) == 0
		? 0 : _Mask));
	}

_MRTIMP2_NCEEPURE const wchar_t * __CLRCALL_PURE_OR_CDECL _Getwctypes(
	const wchar_t *_First, const wchar_t *_Last,
		short *_Dest, const _Ctypevec *_Ctype)
	{	/* get mask sequence for elements in [_First, _Last) */
	__crtGetStringTypeW(NULL, CT_CTYPE1, _First, (int)(_Last - _First),
		(LPWORD)_Dest, _Ctype->_Page, _Ctype->_Hand);
	return (_Last);
	}

#ifdef MRTDLL
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _Getwctype(unsigned short _Ch,
	const _Ctypevec *_Ctype)
    {
    return _Getwctype((wchar_t) _Ch, _Ctype);
    }

_MRTIMP2_NCEEPURE const unsigned short * __CLRCALL_PURE_OR_CDECL _Getwctypes(
	const unsigned short *_First, const unsigned short *_Last,
		short *_Dest, const _Ctypevec *_Ctype)
    {
    return (const unsigned short *)_Getwctypes((wchar_t *)_First, (wchar_t *) _Last,
                       _Dest, _Ctype);
    }
#endif

_C_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
