// xlock.cpp -- global lock for locales, etc.
#include <stdlib.h>
#include <yvals.h>

 #if _MULTI_THREAD
  #include "xmtx.h"
_STD_BEGIN

  #define MAX_LOCK	4	/* must be power of two */

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

static _Rmtx mtx[MAX_LOCK];
static long init = -1;

#if !defined(MRTDLL)

__thiscall _Init_locks::_Init_locks()
	{	// initialize locks
	if (InterlockedIncrement(&init) == 0)
		for (int count = 0; count < MAX_LOCK; ++count)
			_Mtxinit(&mtx[count]);
	}

__thiscall _Init_locks::~_Init_locks()
	{	// clean up locks
	if (InterlockedDecrement(&init) < 0)
		for (int count = 0; count < MAX_LOCK; ++count)
			_Mtxdst(&mtx[count]);
	}

#endif

_MRTIMP2_NPURE void __cdecl _Init_locks::_Init_locks_ctor(_Init_locks * _This)
	{	// initialize locks
	if (InterlockedIncrement(&init) == 0)
		for (int count = 0; count < MAX_LOCK; ++count)
			_Mtxinit(&mtx[count]);
	}

_MRTIMP2_NPURE void __cdecl _Init_locks::_Init_locks_dtor(_Init_locks * _This)
	{	// clean up locks
	if (InterlockedDecrement(&init) < 0)
		for (int count = 0; count < MAX_LOCK; ++count)
			_Mtxdst(&mtx[count]);
	}

static _Init_locks initlocks;
 
#if !defined(MRTDLL)

__thiscall _Lockit::_Lockit()
	: _Locktype(0)

	{	// lock default mutex
	_Mtxlock(&mtx[0]);
	}

__thiscall _Lockit::_Lockit(int kind)
	: _Locktype(kind & (MAX_LOCK - 1))
	{	// lock the mutex
	_Mtxlock(&mtx[_Locktype]);

	}

__thiscall _Lockit::~_Lockit()
	{	// unlock the mutex
	_Mtxunlock(&mtx[_Locktype]);
	}

#endif

_MRTIMP2_NPURE void __cdecl _Lockit::_Lockit_ctor(_Lockit * _This)
	{	// lock default mutex
	_Mtxlock(&mtx[0]);
	}

_MRTIMP2_NPURE void __cdecl _Lockit::_Lockit_ctor(_Lockit * _This, int kind)
	{	// lock the mutex
     _This->_Locktype = kind & (MAX_LOCK - 1);
	_Mtxlock(&mtx[_This->_Locktype]);
	}

_MRTIMP2_NPURE void __cdecl _Lockit::_Lockit_dtor(_Lockit * _This)
	{	// unlock the mutex
	_Mtxunlock(&mtx[_This->_Locktype]);
	}

_RELIABILITY_CONTRACT
_MRTIMP2_NPURE void __cdecl _Lockit::_Lockit_ctor(int kind)
	{	// lock the mutex
	_Mtxlock(&mtx[kind & (MAX_LOCK - 1)]);
	}

_RELIABILITY_CONTRACT
_MRTIMP2_NPURE void __cdecl _Lockit::_Lockit_dtor(int kind)
	{	// unlock the mutex
	_Mtxunlock(&mtx[kind & (MAX_LOCK - 1)]);
	}

_STD_END
 #endif	// _MULTI_THREAD

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
