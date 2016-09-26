/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Fri Mar 20 12:55:54 1998
 */
/* Compiler settings for Chapter10_GITServer.idl:
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


const IID IID_IMath = {0xC2C66744, 0xC012, 0x11D1, {0xB6, 0x19, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


const IID LIBID_CHAPTER10_GITSERVERLib = {0xC2C66733, 0xC012, 0x11D1, {0xB6, 0x19, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


const CLSID CLSID_Math = {0xC2C66745, 0xC012, 0x11D1, {0xB6, 0x19, 0x00, 0x04, 0xAC, 0xFF, 0x17, 0x1C}};


#ifdef __cplusplus
}
#endif

