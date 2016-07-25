/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    simpldrv.h

Abstract:

    This module contains the PRIVATE (driver-only) definitions for the
    code that implements the mono device driver.

Environment:

    Kernel & user mode

Revision History:

    06-25-93 : created

--*/



//
// A structure representing the instance information associated with
// a particular device
//

typedef struct _DEVICE_EXTENSION
{
    ULONG  StateVariable;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



//
// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
//

#define FILE_DEVICE_SIMPLDRV  0x00008300



//
// Macro definition for defining IOCTL and FSCTL function control codes.  Note
// that function codes 0-2047 are reserved for Microsoft Corporation, and
// 2048-4095 are reserved for customers.
//

#define SIMPLDRV_IOCTL_INDEX  0x830



//
// The MONO device driver IOCTLs
//

#define IOCTL_SIMPLDRV_HELLO          CTL_CODE(FILE_DEVICE_SIMPLDRV,  \
                                               SIMPLDRV_IOCTL_INDEX,  \
                                               METHOD_BUFFERED,       \
                                               FILE_ANY_ACCESS)
