
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 6.00.0347 */
/* at Thu Nov 21 08:29:25 2002
 */
/* Compiler settings for SpeakerNoAttr.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)

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

MIDL_DEFINE_GUID(IID, IID_ISpeaker, 0xCE00527D, 0xF8E3, 0x46A3, 0x8B, 0xC8, 0xA2, 0x53, 0x45, 0xAD, 0x6C, 0xAA);


MIDL_DEFINE_GUID(IID, IID_IListener, 0xBECC670E, 0x7A07, 0x4032, 0xA5, 0xA3, 0x07, 0xD1, 0x55, 0x8A, 0xE6, 0x5D);


MIDL_DEFINE_GUID(IID, IID_INamedObject, 0x450D6780, 0xFC73, 0x11d1, 0x8C, 0xC3, 0x00, 0xA0, 0xC9, 0xC8, 0xE5, 0x0D);


MIDL_DEFINE_GUID(IID, LIBID_SpeakerNoAttrLib, 0x6C353116, 0x9E91, 0x4429, 0xA6, 0xEC, 0xA6, 0xDC, 0x65, 0xAF, 0xAD, 0xCC);


MIDL_DEFINE_GUID(IID, DIID__ISpeakerEvents, 0x9CB7F78F, 0xEF31, 0x4702, 0x95, 0x10, 0x72, 0xB0, 0x95, 0x2D, 0xF4, 0x87);


MIDL_DEFINE_GUID(CLSID, CLSID_Demagogue, 0xBA5A5622, 0x05DF, 0x45CF, 0xA8, 0x0A, 0xE0, 0x9D, 0xC1, 0x45, 0xB0, 0x1C);


MIDL_DEFINE_GUID(CLSID, CLSID_EarPolitic, 0x3BEA68EC, 0x4B6D, 0x46D7, 0xA3, 0xEE, 0x79, 0x2C, 0xA1, 0x0E, 0xB0, 0xF6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

