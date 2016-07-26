// locale -- class locale member functions

// In debug, force the instantiation of the string classes with _DEFINE_EMPTY_STRING_CONSTRUCTOR.
// This will force basic_string::basic_string(void) to be exported by the dll, avoiding bincompa problems
// See VSW#449741 and VSW#426913
#if _DEBUG
#define __FORCE_INSTANCE
#define _DEFINE_EMPTY_STRING_CONSTRUCTOR
#include <xstring>
#undef __FORCE_INSTANCE
#undef _DEFINE_EMPTY_STRING_CONSTRUCTOR
#endif

#include <cstdlib>
#include <istream>
#include <xlocale>
#include <xdebug>
#include <dbgint.h>

 #pragma warning(disable: 4074)
 #pragma init_seg(compiler)

  #include <dbgint.h>

 #ifndef _CAT_TO_LC
  #define _CAT_TO_LC(cat)	(cat)
 #endif /* _CAT_TO_LC */

_STD_BEGIN

typedef char_traits<char> _Traits;
typedef istreambuf_iterator<char, _Traits> _Initer;
typedef ostreambuf_iterator<char, _Traits> _Outiter;

_MRTIMP2 locale __cdecl locale::global(const locale& loc)
	{	// change global locale
	locale _Oldglobal;
	_BEGIN_LOCK(_LOCK_LOCALE)
		locale::_Locimp *_Ptr = _Getgloballocale();

		if (_Ptr != loc._Ptr)
			{	// set new global locale
			_DELETE_CRT(_Ptr->_Decref());
			_Setgloballocale(_Ptr = loc._Ptr);
			_Ptr->_Incref();
			category _Cmask = _Ptr->_Catmask & all;
			if (_Cmask == all)
				setlocale(LC_ALL, _Ptr->_Name.c_str());
			else
				for (int catindex = 0; catindex <= _X_MAX; ++catindex)
					if ((_CATMASK(catindex) & _Cmask) != 0)
						setlocale(_CAT_TO_LC(catindex), _Ptr->_Name.c_str());
			}
		return (_Oldglobal);
	_END_LOCK()
	}

	// facets associated with C categories
#define ADDFAC(Facet, cat, ptrimp, ptrloc) \
	if ((_CATMASK(Facet::_Getcat()) & cat) == 0) \
		; \
	else if (ptrloc == 0) \
		ptrimp->_Addfac(_NEW_CRT Facet(lobj), Facet::id); \
	else \
		ptrimp->_Addfac((locale::facet *)&_USE(*ptrloc, Facet), Facet::id);

typedef ctype<char> _T1;
typedef num_get<char, _Initer> _T2;
typedef num_put<char, _Outiter> _T3;
typedef numpunct<char> _T4;
// others moved to wlocale and xlocale to ease subsetting
typedef codecvt<char, char, _Mbstatet> _Tc1;


_MRTIMP2 locale::_Locimp *__cdecl locale::_Locimp::_Makeloc(const _Locinfo& lobj,
	locale::category cat, _Locimp *ptrimp, const locale *ptrloc)
	{	// setup a new locale
	ADDFAC(_T1, cat, ptrimp, ptrloc);
	ADDFAC(_T2, cat, ptrimp, ptrloc);
	ADDFAC(_T3, cat, ptrimp, ptrloc);
	ADDFAC(_T4, cat, ptrimp, ptrloc);
	//...
	ADDFAC(_Tc1, cat, ptrimp, ptrloc);

	_Locimp::_Makexloc(lobj, cat, ptrimp, ptrloc);
	_Locimp::_Makewloc(lobj, cat, ptrimp, ptrloc);
#ifdef _NATIVE_WCHAR_T_DEFINED
	_Locimp::_Makeushloc(lobj, cat, ptrimp, ptrloc);
#endif
	ptrimp->_Catmask |= cat;
	ptrimp->_Name = lobj._Getname();
	return (ptrimp);
	}

_MRTIMP2 void __cdecl locale::_Locimp::_Locimp_ctor(locale::_Locimp *_This, const locale::_Locimp& imp)
	{	// construct a _Locimp from a copy
	if (&imp == _This->_Clocptr)
		{
		_BEGIN_LOCINFO(_Lobj)
			_Makeloc(_Lobj, locale::all, _This, 0);
		_END_LOCINFO()
		}
	else
		{	// lock to keep facets from disappearing
		_BEGIN_LOCK(_LOCK_LOCALE)
			if (0 < _This->_Facetcount)
				{	// copy over nonempty facet vector
				if ((_This->_Facetvec = (locale::facet **)_malloc_crt(
					_This->_Facetcount * sizeof (locale::facet *))) == 0)
					{	// report no memory
					static const _XSTD bad_alloc nomem;
					_RAISE(nomem);
					}
				for (size_t count = _This->_Facetcount; 0 < count; )
					{	// copy over facet pointers
					locale::facet *ptrfac = imp._Facetvec[--count];
					if ((_This->_Facetvec[count] = ptrfac) != 0)
						ptrfac->_Incref();
					}
				}
		_END_LOCK()
		}
	}

_MRTIMP2 void __cdecl locale::_Locimp::_Locimp_Addfac(_Locimp *_This, locale::facet *ptrfac, size_t id)
	{	// add a facet to a locale
	_BEGIN_LOCK(_LOCK_LOCALE)
		const size_t MINCAT = 40;	// minimum number of facets in a locale

		if (_This->_Facetcount <= id)
			{	// make facet vector larger
			size_t count = id + 1;
			if (count < MINCAT)
				count = MINCAT;
			locale::facet **ptrnewvec = (locale::facet **)_realloc_crt(_This->_Facetvec,
				count * sizeof (locale::facet **));
			if (ptrnewvec == 0)
				{	// report no memory
				static const _XSTD bad_alloc nomem;
				_RAISE(nomem);
				}

			_This->_Facetvec = ptrnewvec;
			for (; _This->_Facetcount < count; ++_This->_Facetcount)
				_This->_Facetvec[_This->_Facetcount] = 0;
			}

		ptrfac->_Incref();
		if (_This->_Facetvec[id] != 0)
			_DELETE_CRT(_This->_Facetvec[id]->_Decref());
		_This->_Facetvec[id] = ptrfac;
	_END_LOCK()
	}

_MRTIMP2_NPURE void __cdecl _Locinfo::_Locinfo_ctor(_Locinfo *pLocinfo, int cat, const char *locname)
	{	// capture a named locale
	const char *oldlocname = setlocale(LC_ALL, 0);

	pLocinfo->_Oldlocname = oldlocname == NULL ? "" : oldlocname;
	_Locinfo_Addcats(pLocinfo, cat, locname);
	}

_MRTIMP2_NPURE _Locinfo& __cdecl _Locinfo::_Locinfo_Addcats(_Locinfo *pLocinfo, int cat, const char *locname)
	{	// merge in another named locale
	const char *oldlocname = 0;

	if (locname[0] == '*' && locname[1] == '\0')
		;
	else if (cat == 0)
		oldlocname = setlocale(LC_ALL, 0);
	else if (cat == _M_ALL)
		oldlocname = setlocale(LC_ALL, locname);
	else
		{	// alter selected categories
		for (int catindex = 0; catindex <= _X_MAX; ++catindex)
			if ((_CATMASK(catindex) & cat) != 0)
				setlocale(_CAT_TO_LC(catindex), locname);
		oldlocname = setlocale(LC_ALL, locname);
		}
	if (oldlocname == 0)
		pLocinfo->_Newlocname = "*";
	else if (pLocinfo->_Newlocname.compare("*") != 0)
		pLocinfo->_Newlocname = oldlocname;
	return (*pLocinfo);
	}
_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
