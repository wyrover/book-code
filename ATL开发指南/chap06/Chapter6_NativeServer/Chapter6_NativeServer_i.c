/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 11 12:24:17 1998
 */
/* Compiler settings for NativeServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
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

const IID IID_IMath = {0xB8721602, 0x4A3D, 0x11d1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID LIBID_NativeServerLib = {0xB8721600, 0x4A3D, 0x11d1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const CLSID CLSID_Math = {0xB8721601, 0x4A3D, 0x11d1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

