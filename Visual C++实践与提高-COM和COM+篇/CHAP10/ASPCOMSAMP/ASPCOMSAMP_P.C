/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 28 11:57:05 2000
 */
/* Compiler settings for C:\Vccom\chap10\ASPComSamp\ASPComSamp.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "ASPComSamp.h"

#define TYPE_FORMAT_STRING_SIZE   3
#define PROC_FORMAT_STRING_SIZE   23

typedef struct _MIDL_TYPE_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
} MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IASPComp, ver. 0.0,
   GUID={0x1474A2ED,0x951F,0x11D4,{0x89,0x6A,0xFD,0x2A,0xD6,0x50,0xDB,0x79}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IASPComp_ServerInfo;

#pragma code_seg(".orpc")

static const MIDL_STUB_DESC Object_StubDesc = {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

static const unsigned short IASPComp_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0
};

static const MIDL_SERVER_INFO IASPComp_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IASPComp_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IASPComp_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IASPComp_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(8) _IASPCompProxyVtbl = {
    &IASPComp_ProxyInfo,
    &IID_IASPComp,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IASPComp::ProcessRequest */
};


static const PRPC_STUB_FUNCTION IASPComp_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IASPCompStubVtbl = {
    &IID_IASPComp,
    &IASPComp_ServerInfo,
    8,
    &IASPComp_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
    #error You need a Windows NT 4.0 or later to run this stub because it uses these features:
    #error   -Oif or -Oicf, more than 32 methods in the interface.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure ProcessRequest */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x4,        /* Oi2 Flags:  has return, */
        0x1,        /* 1 */

        /* Return value */

        /* 16 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */

        0x0
    }
};

const CInterfaceProxyVtbl * _ASPComSamp_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IASPCompProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ASPComSamp_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IASPCompStubVtbl,
    0
};

PCInterfaceName const _ASPComSamp_InterfaceNamesList[] = {
    "IASPComp",
    0
};

const IID *  _ASPComSamp_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _ASPComSamp_CHECK_IID(n)    IID_GENERIC_CHECK_IID( _ASPComSamp, pIID, n)

int __stdcall _ASPComSamp_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_ASPComSamp_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo ASPComSamp_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _ASPComSamp_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ASPComSamp_StubVtblList,
    (const PCInterfaceName *) & _ASPComSamp_InterfaceNamesList,
    (const IID **) & _ASPComSamp_BaseIIDList,
    & _ASPComSamp_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
