/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Nov 13 12:36:59 1999
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\chap07\Chapter7_CPServer\Chapter7_CPServer.idl:
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

const IID IID_IMath = {0x99D952C7, 0x99E6, 0x11D3, {0x83, 0x66, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const IID LIBID_CHAPTER7_CPSERVERLib = {0x99D952BB, 0x99E6, 0x11D3, {0x83, 0x66, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const IID DIID__IMathEvents = {0x99D952C8, 0x99E6, 0x11D3, {0x83, 0x66, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const CLSID CLSID_Math = {0xA4FA55EE, 0xB221, 0x11D2, {0x97, 0xC1, 0x00, 0x10, 0x4B, 0xF7, 0xDE, 0x35}};


#ifdef __cplusplus
}
#endif

