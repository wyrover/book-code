/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Fri Nov 28 06:30:51 1997
 */
/* Compiler settings for Chapter10_Server.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C" {
#endif


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID {
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ICallBack = {0x9029D3B0, 0x67FE, 0x11d1, {0xB5, 0xF9, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


const IID IID_IMath = {0xEBFEC172, 0x67FA, 0x11D1, {0xB5, 0xF9, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


const IID LIBID_CHAPTER10_SERVERLib = {0xEBFEC163, 0x67FA, 0x11D1, {0xB5, 0xF9, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


const CLSID CLSID_Math = {0xEBFEC173, 0x67FA, 0x11D1, {0xB5, 0xF9, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


#ifdef __cplusplus
}
#endif

