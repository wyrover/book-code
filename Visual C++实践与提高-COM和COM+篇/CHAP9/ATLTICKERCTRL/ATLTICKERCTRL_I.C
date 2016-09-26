/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Sep 19 14:54:19 2000
 */
/* Compiler settings for C:\VcCom\chap9\AtlTickerCtrl\AtlTickerCtrl.idl:
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

const IID IID_ITicker = {0x48727BC4, 0xB815, 0x4EBB, {0x97, 0x18, 0x84, 0xEF, 0xF6, 0xD5, 0x95, 0x05}};


const IID LIBID_ATLTICKERCTRLLib = {0xB3002585, 0x04C1, 0x49D0, {0xB0, 0xF0, 0xB1, 0x3E, 0x69, 0x99, 0xF1, 0x3E}};


const IID DIID__ITickerEvents = {0x43401211, 0xFF77, 0x41E0, {0xB6, 0x59, 0x17, 0x63, 0x6F, 0x11, 0x9F, 0x57}};


const CLSID CLSID_Ticker = {0xCB9E8D82, 0xBC1E, 0x4865, {0xAD, 0xD3, 0x7C, 0xC1, 0x67, 0x8E, 0xA7, 0x12}};


const CLSID CLSID_TickerPP = {0xA5ACC474, 0x4C00, 0x4988, {0xB2, 0x15, 0x09, 0x54, 0x1C, 0x57, 0x40, 0xC8}};


#ifdef __cplusplus
}
#endif

