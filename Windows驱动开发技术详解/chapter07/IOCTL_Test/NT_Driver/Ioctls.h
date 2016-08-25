// IOCTLS.H -- IOCTL code definitions for fileio driver
// Copyright (C) 1999 by Walter Oney
// All rights reserved

#ifndef IOCTLS_H
#define IOCTLS_H

#ifndef CTL_CODE
	#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

#define IOCTL_TEST1 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x800, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

#define IOCTL_TEST2 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x801, \
			METHOD_IN_DIRECT, \
			FILE_ANY_ACCESS)

#define IOCTL_TEST3 CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x802, \
			METHOD_NEITHER, \
			FILE_ANY_ACCESS)

#endif
