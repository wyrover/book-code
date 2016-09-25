/* config.h.  Generated from config.h.in by configure.  */
/*
** 2008 March 6
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Configuration header template to be filled in by 'configure' script
**
** @(#) $Id: config.h.in,v 1.3 2008/07/22 05:05:02 shane Exp $
*/
#ifndef _CONFIG_H_
#define _CONFIG_H_


/*****************************
** Data types
*****************************/

/* Define as 1 if you have the int8_t type */
#define HAVE_INT8_T 1

/* Define as 1 if you have the int16_t type */
#define HAVE_INT16_T 1

/* Define as 1 if you have the int32_t type */
#define HAVE_INT32_T 1

/* Define as 1 if you have the int64_t type */
#define HAVE_INT64_T 1

/* Define as 1 if you have the uint8_t type */
#define HAVE_UINT8_T 1

/* Define as 1 if you have the uint16_t type */
#define HAVE_UINT16_T 1

/* Define as 1 if you have the uint32_t type */
#define HAVE_UINT32_T 1

/* Define as 1 if you have the uint64_t type */
#define HAVE_UINT64_T 1


/*****************************
** Header Files
*****************************/

/* Define as 1 if you have the sys/types.h header */
#define HAVE_SYS_TYPES_H 1

/* Define as 1 if you have the stdlib.h header */
#define HAVE_STDLIB_H 1

/* Define as 1 if you have the stdint.h header */
#define HAVE_STDINT_H 1

/* Define as 1 if you have the inttypes.h header */
#define HAVE_INTTYPES_H 1


/*****************************
** Functions
*****************************/

/* Define as 1 if you have the usleep() function */
#define HAVE_USLEEP 1

/* Define as 1 if you have the fdatasync() function */
#define HAVE_FDATASYNC 1

/* Define as 1 if you have the gmtime_r() function */
#define HAVE_GMTIME_R 1

/* Define as 1 if you have the localtime_r() function */
/* #define HAVE_LOCALTIME_R 1 */

/* Define as 1 if you have the localtime_s() function */
/* #undef HAVE_LOCALTIME_S */


/*****************************
** Large file support
*****************************/

/* #undef _FILE_OFFSET_BITS */
/* #undef _LARGE_FILES */


/* End of header */
#endif
