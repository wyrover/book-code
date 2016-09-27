/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Dec 14 13:40:11 2000
 */
/* Compiler settings for D:\ATLSampleExe\ATLSampleExe.idl:
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

const IID IID_IAccount = {0x6EA21C2C, 0xD1C4, 0x11D4, {0xA2, 0x39, 0x50, 0x37, 0x48, 0xC1, 0x00, 0x00}};


const IID LIBID_ATLSAMPLEEXELib = {0x6EA21C20, 0xD1C4, 0x11D4, {0xA2, 0x39, 0x50, 0x37, 0x48, 0xC1, 0x00, 0x00}};


const CLSID CLSID_Account = {0x6EA21C2D, 0xD1C4, 0x11D4, {0xA2, 0x39, 0x50, 0x37, 0x48, 0xC1, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

