/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Nov 05 14:54:02 1999
 */
/* Compiler settings for D:\My Documents\ATL_OLEDB_Prov\ATL_OLEDB_Prov.idl:
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

const IID IID_IDBCarusoeProvSource = {0x8DFCA8E0, 0x791B, 0x11d3, {0x9D, 0x83, 0xD7, 0x98, 0x26, 0x59, 0x41, 0x33}};


const IID LIBID_ATL_OLEDB_PROVLib = {0xF5F87941, 0x9374, 0x11D3, {0x85, 0x0F, 0x00, 0x40, 0x05, 0x30, 0xCF, 0xB7}};


const CLSID CLSID_CarusoeProv = {0x4DAF1613, 0x783B, 0x11D3, {0x9D, 0x83, 0xDA, 0x68, 0xCA, 0x73, 0x78, 0x33}};


#ifdef __cplusplus
}
#endif

