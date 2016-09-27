/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Feb 06 15:56:08 2000
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap09\Chapter9_EnumServer\Chapter9_EnumServer.idl:
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

const IID IID_IBasicEnum = {0xDBD4D8EF, 0xDCD4, 0x11D3, {0x83, 0xC0, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const IID LIBID_CHAPTER9_ENUMSERVERLib = {0xDBD4D8E3, 0xDCD4, 0x11D3, {0x83, 0xC0, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const CLSID CLSID_BasicEnum = {0xDBD4D8F0, 0xDCD4, 0x11D3, {0x83, 0xC0, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


#ifdef __cplusplus
}
#endif

