//cal.h

#ifdef DLL_SOURCE
	#define DLLFUNC	__declspec(dllexport)   
#else
	#define DLLFUNC __declspec(dllimport)
#endif

#include <windows.h>

EXTERN_C DLLFUNC int Add( int a, int b);
EXTERN_C DLLFUNC int Sub( int a, int b);
EXTERN_C DLLFUNC int Mul( int a, int b);
EXTERN_C DLLFUNC int Div( int a, int b);
//--------------------------------------------------------