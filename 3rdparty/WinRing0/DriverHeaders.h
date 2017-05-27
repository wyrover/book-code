/*-----------------------------------------------------------------------------
 * CommonHeaders.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * - 10.11.2010 created
**---------------------------------------------------------------------------*/

#ifndef _common_headers_h_
#define _common_headers_h_

#ifdef __cplusplus
extern "C" {
#endif 

#include <ntifs.h>
#include <ntstrsafe.h>
#include <ntintsafe.h>

#ifdef __cplusplus
}
#endif 



#include "DriverDebug.h"

typedef struct _DEVICE_EXTENSION 
{
	PDEVICE_OBJECT		device_object;// device object this extension belongs to	


	BOOLEAN				initialized;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

extern PDEVICE_EXTENSION		g_dev_ext;

#endif//_common_headers_h_