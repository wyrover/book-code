#ifndef _ASSERT_H
#define _ASSERT_H 1

#ifndef _INC_CRTDBG
	#include <crtdbg.h>
#endif

#pragma warning(disable: 4127)  // constant expression for TRACE/ASSERT
#pragma warning(disable: 4201)  // nameless unions are part of C++

#ifndef ASSERT
#define ASSERT _ASSERTE
#endif

#ifndef VERIFY
	#ifdef _DEBUG
	#define VERIFY _ASSERTE
	#else
	#define VERIFY(f) ((void)(f))
	#endif
#endif

#endif

