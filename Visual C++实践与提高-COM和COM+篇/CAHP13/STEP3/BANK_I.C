/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Aug 27 12:25:30 2000
 */
/* Compiler settings for D:\Samples\step2\Bank\Bank.idl:
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

const IID IID_IAccount = {0x5939F230, 0x7B5F, 0x11D4, {0x8B, 0xEC, 0x34, 0x8F, 0xBB, 0x00, 0x00, 0x00}};


const IID LIBID_BANKLib = {0x5939F221, 0x7B5F, 0x11D4, {0x8B, 0xEC, 0x34, 0x8F, 0xBB, 0x00, 0x00, 0x00}};


const CLSID CLSID_Account = {0x5939F232, 0x7B5F, 0x11D4, {0x8B, 0xEC, 0x34, 0x8F, 0xBB, 0x00, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

