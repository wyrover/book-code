// locale0 -- class locale basic member functions
#include <climits>
#include <locale>
#include <xdebug>

#pragma warning(disable:4439)	// C4439: function with a managed parameter must have a __clrcall calling convention

_STD_BEGIN
struct _Fac_node
	{	// node for lazy facet recording
	__CLR_OR_THIS_CALL _Fac_node(_Fac_node *_Nextarg, std::locale::facet *_Facptrarg)
		: _Next(_Nextarg), _Facptr(_Facptrarg)
		{	// construct a node with value
		}

	__CLR_OR_THIS_CALL ~_Fac_node()
		{	// destroy a facet
		_DELETE_CRT(_Facptr->_Decref());
		}

	_Fac_node *_Next;
	std::locale::facet *_Facptr;
	};

__PURE_APPDOMAIN_GLOBAL static _Fac_node *_Fac_head = 0;
_STD_END

_C_STD_BEGIN
_EXTERN_C
void __cdecl _Fac_tidy()
	{	// destroy lazy facets
	_BEGIN_LOCK(_LOCK_LOCALE)	// prevent double delete
		for (; std::_Fac_head != 0; )
			{	// destroy a lazy facet node
			std::_Fac_node *nodeptr = std::_Fac_head;
			std::_Fac_head = nodeptr->_Next;
			_DELETE_CRT(nodeptr);
			}
	_END_LOCK()
	}

void __CLRCALL_OR_CDECL _Deletegloballocale(void *ptr)
	{	// delete a global locale reference
	std::locale::_Locimp *locptr = *(std::locale::_Locimp **)ptr;
	if (locptr != 0)
		_DELETE_CRT(locptr->_Decref());
	}

__PURE_APPDOMAIN_GLOBAL static std::locale::_Locimp *global_locale = 0;	// pointer to current locale

static void __cdecl tidy_global()
	{	// delete static global locale reference
	_BEGIN_LOCK(_LOCK_LOCALE)	// prevent double delete
		_Deletegloballocale(&global_locale);
		global_locale=NULL;
	_END_LOCK()
	}

_END_EXTERN_C
_C_STD_END

_STD_BEGIN

_MRTIMP2 std::locale::_Locimp *__cdecl std::locale::_Getgloballocale()
	{	// return pointer to current locale
	return (global_locale);
	}

_MRTIMP2 void std::locale::_Setgloballocale(void *ptr)
	{	// alter pointer to current locale
	__PURE_APPDOMAIN_GLOBAL static bool registered = false;

	if (!registered)
		{	// register cleanup first time
		registered = true;
		::_Atexit(&tidy_global);
		}
	global_locale = (std::locale::_Locimp *)ptr;
	}

 #pragma warning(disable: 4786)

__PURE_APPDOMAIN_GLOBAL static locale classic_locale(_Noinit);	// "C" locale object, uninitialized

__PURE_APPDOMAIN_GLOBAL locale::_Locimp *locale::_Locimp::_Clocptr = 0;	// pointer to classic_locale

_MRTIMP2 locale::_Locimp*& __cdecl locale::_Locimp::_Clocptr_func()
{
    return locale::_Locimp::_Clocptr;
}

__PURE_APPDOMAIN_GLOBAL int locale::id::_Id_cnt = 0;	// unique id counter for facets

_MRTIMP2 int& __cdecl locale::id::_Id_cnt_func()
{
    return locale::id::_Id_cnt;
}

__PURE_APPDOMAIN_GLOBAL locale::id ctype<char>::id(0);

_MRTIMP2 locale::id& __cdecl ctype<char>::_Id_func()
{
    return ctype<char>::id;
}

__PURE_APPDOMAIN_GLOBAL locale::id ctype<wchar_t>::id(0);

_MRTIMP2 locale::id& __cdecl ctype<wchar_t>::_Id_func()
{
    return ctype<wchar_t>::id;
}

__PURE_APPDOMAIN_GLOBAL locale::id codecvt<wchar_t, char, mbstate_t>::id(0);

_MRTIMP2 locale::id& __cdecl codecvt<wchar_t, char, mbstate_t>::_Id_func()
{
    return codecvt<wchar_t, char, mbstate_t>::id;
}


 #ifdef _NATIVE_WCHAR_T_DEFINED
__PURE_APPDOMAIN_GLOBAL locale::id ctype<unsigned short>::id(0);

_MRTIMP2 locale::id& __cdecl ctype<unsigned short>::_Id_func()
{
    return ctype<unsigned short>::id;
}

__PURE_APPDOMAIN_GLOBAL locale::id codecvt<unsigned short, char, mbstate_t>::id(0);
_MRTIMP2 locale::id& __cdecl codecvt<unsigned short, char, mbstate_t>::_Id_func()
{
    return codecvt<unsigned short, char, mbstate_t>::id;
}

 #endif /* _NATIVE_WCHAR_T_DEFINED */

_MRTIMP2 const locale& __cdecl locale::classic()
	{	// get reference to "C" locale
	_Init();
	return (classic_locale);
	}

_MRTIMP2 locale __cdecl locale::empty()
	{	// make empty transparent locale
	_Init();
	return (locale(_NEW_CRT _Locimp(true)));
	}

_MRTIMP2 locale::_Locimp *__cdecl locale::_Init()
	{	// setup global and "C" locales
	locale::_Locimp *ptr = _Getgloballocale();
	if (ptr == 0)
		{	// lock and test again
		_BEGIN_LOCK(_LOCK_LOCALE)	// prevent double initialization

			ptr = _Getgloballocale();
			if (ptr == 0)
				{	// create new locales
				_Setgloballocale(ptr = _NEW_CRT _Locimp);
				ptr->_Catmask = all;	// set current locale to "C"
				ptr->_Name = "C";

				_Locimp::_Clocptr = ptr;	// set classic to match
				_Locimp::_Clocptr->_Incref();
				new (&classic_locale) locale(_Locimp::_Clocptr);
				}
		_END_LOCK()
		}
	return (ptr);
	}

_MRTIMP2 void __cdecl locale::facet::facet_Register(locale::facet *_This)
	{	// queue up lazy facet for destruction
	if (_Fac_head == 0)
		::_Atexit(&_Fac_tidy);

	_Fac_head = _NEW_CRT _Fac_node(_Fac_head, _This);
	}

_MRTIMP2 void __cdecl locale::_Locimp::_Locimp_dtor(_Locimp* _This)
	{	// destruct a _Locimp
	_BEGIN_LOCK(_LOCK_LOCALE)	// prevent double delete
		for (size_t count = _This->_Facetcount; 0 < count; )
			if (_This->_Facetvec[--count] != 0)
				_DELETE_CRT(_This->_Facetvec[count]->_Decref());
		free(_This->_Facetvec);
	_END_LOCK()
	}

_MRTIMP2 void __cdecl _Locinfo::_Locinfo_ctor(_Locinfo *pLocinfo, const char *locname)
	{	// switch to a named locale
	const char *oldlocname = setlocale(LC_ALL, 0);

	pLocinfo->_Oldlocname = oldlocname == NULL ? "" : oldlocname;
	if (locname != 0)
		locname = setlocale(LC_ALL, locname);
	pLocinfo->_Newlocname = locname == 0 ? "*" : locname;
	}

_MRTIMP2 void __cdecl _Locinfo::_Locinfo_dtor(_Locinfo *pLocinfo)
	{	// destroy a _Locinfo object, revert locale
	if (0 < pLocinfo->_Oldlocname.size())
		setlocale(LC_ALL, pLocinfo->_Oldlocname.c_str());
	}
_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
