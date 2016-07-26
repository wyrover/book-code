/***
*sect_attribs.h - section attributes for IA64 CRTs
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
****/

#if _MSC_FULL_VER >= 140050214

#define _ATTRIBUTES read

#else  /* _MSC_FULL_VER >= 140050214 */

#if defined (_M_IA64) || defined (_M_AMD64)
#define _ATTRIBUTES read
#else  /* defined (_M_IA64) || defined (_M_AMD64) */
#define _ATTRIBUTES read,write
#endif  /* defined (_M_IA64) || defined (_M_AMD64) */

#endif  /* _MSC_FULL_VER >= 140050214 */


#pragma section(".CRTMP$XCA",long,_ATTRIBUTES)
#pragma section(".CRTMP$XCZ",long,_ATTRIBUTES)
#pragma section(".CRTMP$XIA",long,_ATTRIBUTES)
#pragma section(".CRTMP$XIZ",long,_ATTRIBUTES)

#pragma section(".CRTMA$XCA",long,_ATTRIBUTES)
#pragma section(".CRTMA$XCZ",long,_ATTRIBUTES)
#pragma section(".CRTMA$XIA",long,_ATTRIBUTES)
#pragma section(".CRTMA$XIZ",long,_ATTRIBUTES)

#pragma section(".CRTVT$XCA",long,_ATTRIBUTES)
#pragma section(".CRTVT$XCZ",long,_ATTRIBUTES)

#pragma section(".CRT$XCA",long,_ATTRIBUTES)
#pragma section(".CRT$XCAA",long,_ATTRIBUTES)
#pragma section(".CRT$XCC",long,_ATTRIBUTES)
#pragma section(".CRT$XCZ",long,_ATTRIBUTES)
#pragma section(".CRT$XDA",long,_ATTRIBUTES)
#pragma section(".CRT$XDC",long,_ATTRIBUTES)
#pragma section(".CRT$XDZ",long,_ATTRIBUTES)
#pragma section(".CRT$XIA",long,_ATTRIBUTES)
#pragma section(".CRT$XIAA",long,_ATTRIBUTES)
#pragma section(".CRT$XIC",long,_ATTRIBUTES)
#pragma section(".CRT$XID",long,_ATTRIBUTES)
#pragma section(".CRT$XIY",long,_ATTRIBUTES)
#pragma section(".CRT$XIZ",long,_ATTRIBUTES)
#pragma section(".CRT$XLA",long,_ATTRIBUTES)
#pragma section(".CRT$XLC",long,_ATTRIBUTES)
#pragma section(".CRT$XLD",long,_ATTRIBUTES)
#pragma section(".CRT$XLZ",long,_ATTRIBUTES)
#pragma section(".CRT$XPA",long,_ATTRIBUTES)
#pragma section(".CRT$XPX",long,_ATTRIBUTES)
#pragma section(".CRT$XPXA",long,_ATTRIBUTES)
#pragma section(".CRT$XPZ",long,_ATTRIBUTES)
#pragma section(".CRT$XTA",long,_ATTRIBUTES)
#pragma section(".CRT$XTB",long,_ATTRIBUTES)
#pragma section(".CRT$XTX",long,_ATTRIBUTES)
#pragma section(".CRT$XTZ",long,_ATTRIBUTES)
#pragma section(".rdata$T",long,read)
#pragma section(".rtc$IAA",long,read)
#pragma section(".rtc$IZZ",long,read)
#pragma section(".rtc$TAA",long,read)
#pragma section(".rtc$TZZ",long,read)

#define _CRTALLOC(x) __declspec(allocate(x))
