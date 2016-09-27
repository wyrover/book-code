/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 28 10:45:33 2000
 */
/* Compiler settings for C:\VCCOM\chap10\DOMSamp\DOMSamp.idl:
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

const IID IID_IDOMCtrl = {0x63D6A83B, 0x94C4, 0x11D4, {0x89, 0x6A, 0x80, 0xFF, 0x10, 0x0A, 0xAD, 0x79}};


const IID LIBID_DOMSAMPLib = {0x63D6A82F, 0x94C4, 0x11D4, {0x89, 0x6A, 0x80, 0xFF, 0x10, 0x0A, 0xAD, 0x79}};


const CLSID CLSID_DOMCtrl = {0x63D6A83C, 0x94C4, 0x11D4, {0x89, 0x6A, 0x80, 0xFF, 0x10, 0x0A, 0xAD, 0x79}};


#ifdef __cplusplus
}
#endif

