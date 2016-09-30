

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 7.00.0555 */
/* at Mon Sep 26 18:23:05 2016
 */
/* Compiler settings for .\ConnectionCOM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C" {
#endif


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

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

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IAdd, 0x295D9570, 0xC579, 0x4696, 0x8B, 0xE2, 0xC0, 0x69, 0xDE, 0xC0, 0x6F, 0x36);


MIDL_DEFINE_GUID(IID, LIBID_CONNECTIONCOMLib, 0x40EAC38A, 0x60AA, 0x4F14, 0xBC, 0x5D, 0xC7, 0xC3, 0xC8, 0xDE, 0x89, 0xC5);


MIDL_DEFINE_GUID(IID, IID__IAddEvents, 0xAFE854B0, 0x246F, 0x4B66, 0xB2, 0x6F, 0xA1, 0x06, 0x02, 0x25, 0xC7, 0x1C);


MIDL_DEFINE_GUID(CLSID, CLSID_Add, 0x630B3CD3, 0xDDB1, 0x43CE, 0xAD, 0x2F, 0x4F, 0x57, 0xDC, 0x54, 0xD5, 0xD0);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



