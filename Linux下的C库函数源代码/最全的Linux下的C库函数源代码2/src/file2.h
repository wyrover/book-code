/***
*file2.h - auxiliary file structure used internally by file run-time routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the auxiliary file structure used internally by
*       the file run time routines.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_FILE2
#define _INC_FILE2

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* Additional _iobuf[]._flag values
 *
 *  _IOSETVBUF - Indicates file was buffered via a setvbuf (or setbuf call).
 *               Currently used ONLY in _filbuf.c, _getbuf.c, fseek.c and
 *               setvbuf.c, to disable buffer resizing on "random access"
 *               files if the buffer was user-installed.
 */

#define _IOYOURBUF      0x0100
#define _IOSETVBUF      0x0400
#define _IOFEOF         0x0800
#define _IOFLRTN        0x1000
#define _IOCTRLZ        0x2000
#define _IOCOMMIT       0x4000


/* General use macros */

#define inuse(s)        ((s)->_flag & (_IOREAD|_IOWRT|_IORW))
#define mbuf(s)         ((s)->_flag & _IOMYBUF)
#define nbuf(s)         ((s)->_flag & _IONBF)
#define ybuf(s)         ((s)->_flag & _IOYOURBUF)
#define bigbuf(s)       ((s)->_flag & (_IOMYBUF|_IOYOURBUF))
#define anybuf(s)       ((s)->_flag & (_IOMYBUF|_IONBF|_IOYOURBUF))

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_FILE2 */
