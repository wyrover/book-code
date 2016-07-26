/***
*nothrow0.cpp - defines object std::nothrow_t for placement new
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*       Derived from code Copyright (c) 1992-2001 by P.J. Plauger.
*
*Purpose:
*       Defines the object std::nothrow which can be used as a placement
*       new tag to call the form of operator new which is guaranteed to
*       return NULL on an allocation failure instead of raising an
*       exception of std::bad_alloc.
*
*******************************************************************************/

#ifdef CRTDLL
#undef CRTDLL
#endif  /* CRTDLL */

#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#include <new.h>

namespace std {

    const nothrow_t nothrow = nothrow_t();

};
