#ifndef uac_h__
#define uac_h__


#ifdef _WIN64
#define WINVER 0x502
#else
#define WINVER 0x400
#endif

#include <windows.h>
#include <aclapi.h>
#include <sddl.h>



#endif // uac_h__
