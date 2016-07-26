//	xmutex.cpp -- implement mutex lock for iostreams
#include <xdebug>
#include <yvals.h>

 #if _MULTI_THREAD && _IOSTREAM_OP_LOCKS
  #include "xmtx.h"
_STD_BEGIN

#if !defined(MRTDLL)
__thiscall _Mutex::_Mutex()
	: _Mtx(_NEW_CRT _Rmtx)
	{	// initialize recursive mutex object
	_Mtxinit((_Rmtx*)_Mtx);
	}

__thiscall _Mutex::~_Mutex()
	{	// release resources allocated to mutex object
	_Mtxdst((_Rmtx*)_Mtx);
	_DELETE_CRT((_Rmtx*)_Mtx);
	}

void __thiscall _Mutex::_Lock()
	{	// lock mutex
	_Mtxlock((_Rmtx*)_Mtx);
	}

void __thiscall _Mutex::_Unlock()
	{	// unlock mutex
	_Mtxunlock((_Rmtx*)_Mtx);
	}
#endif // !defined(MRTDLL)

_MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex::_Mutex_ctor(_Mutex * _This)
	{	// initialize recursive mutex object
    _This->_Mtx = _NEW_CRT _Rmtx;
	_Mtxinit((_Rmtx*)_This->_Mtx);
	}

_MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex::_Mutex_dtor(_Mutex * _This)
	{	// release resources allocated to mutex object
	_Mtxdst((_Rmtx*)_This->_Mtx);
	_DELETE_CRT((_Rmtx*)_This->_Mtx);
	}

_MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex::_Mutex_Lock(_Mutex * _This)
	{	// lock mutex
	_Mtxlock((_Rmtx*)_This->_Mtx);
	}

_MRTIMP2_NPURE_NCEEPURE void __CLRCALL_PURE_OR_CDECL _Mutex::_Mutex_Unlock(_Mutex * _This)
	{	// unlock mutex
	_Mtxunlock((_Rmtx*)_This->_Mtx);
	}
_STD_END
 #endif	/* _MULTI_THREAD */

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
