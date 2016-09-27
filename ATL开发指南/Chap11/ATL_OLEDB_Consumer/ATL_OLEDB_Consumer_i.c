/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Nov 10 17:42:16 1999
 */
/* Compiler settings for D:\My Documents\ATL_OLEDB_Consumer\ATL_OLEDB_Consumer.idl:
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

const IID LIBID_ATL_OLEDB_ConsumerLib = {0xE4658771, 0x97AE, 0x11D3, {0x85, 0x1B, 0x00, 0x40, 0x05, 0x30, 0xCF, 0xB7}};


#ifdef __cplusplus
}
#endif

