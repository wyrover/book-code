/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Sep 02 12:16:59 2000
 */
/* Compiler settings for D:\samples\chap5\ATLSample\ATLSample.idl:
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

const IID IID_IAccount = {0x3B4F8E7A, 0x80C1, 0x11D4, {0xA2, 0x3D, 0x02, 0x60, 0x8C, 0xAC, 0xED, 0xBB}};


const IID LIBID_ATLSAMPLELib = {0x3B4F8E6E, 0x80C1, 0x11D4, {0xA2, 0x3D, 0x02, 0x60, 0x8C, 0xAC, 0xED, 0xBB}};


const CLSID CLSID_Account = {0x3B4F8E7B, 0x80C1, 0x11D4, {0xA2, 0x3D, 0x02, 0x60, 0x8C, 0xAC, 0xED, 0xBB}};


#ifdef __cplusplus
}
#endif

