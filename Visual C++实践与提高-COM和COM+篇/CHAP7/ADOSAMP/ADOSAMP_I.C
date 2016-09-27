/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Sep 04 20:47:57 2000
 */
/* Compiler settings for D:\Samples\myADOSamp\ADOSamp.idl:
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

const IID IID_IAccount = {0x031A067F, 0x825B, 0x11D4, {0x8C, 0x0E, 0x44, 0x62, 0x60, 0x00, 0x00, 0x00}};


const IID LIBID_ADOSAMPLib = {0x031A0671, 0x825B, 0x11D4, {0x8C, 0x0E, 0x44, 0x62, 0x60, 0x00, 0x00, 0x00}};


const CLSID CLSID_Account = {0x031A0680, 0x825B, 0x11D4, {0x8C, 0x0E, 0x44, 0x62, 0x60, 0x00, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

