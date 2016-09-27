/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Feb 06 15:56:08 2000
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap09\Chapter9_EnumServer\Chapter9_EnumServer.idl:
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


#include "Chapter9_EnumServer.h"

#define TYPE_FORMAT_STRING_SIZE   25
#define PROC_FORMAT_STRING_SIZE   29

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


/* Object interface: IBasicEnum, ver. 0.0,
   GUID={0xDBD4D8EF,0xDCD4,0x11D3,{0x83,0xC0,0x00,0x60,0x08,0x1A,0xEB,0x5F}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IBasicEnum_ServerInfo;

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

static const unsigned short IBasicEnum_FormatStringOffsetTable[] = {
    0
};

static const MIDL_SERVER_INFO IBasicEnum_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IBasicEnum_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IBasicEnum_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IBasicEnum_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(4) _IBasicEnumProxyVtbl = {
    &IBasicEnum_ProxyInfo,
    &IID_IBasicEnum,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IBasicEnum::get_Enum */
};

const CInterfaceStubVtbl _IBasicEnumStubVtbl = {
    &IID_IBasicEnum,
    &IBasicEnum_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
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

        /* Procedure get_Enum */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter ppVal */

        /* 16 */    NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    NdrFcShort(0x2),     /* Type Offset=2 */

        /* Return value */

        /* 22 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 24 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x11, 0x10, /* FC_RP */
        /*  4 */    NdrFcShort(0x2),     /* Offset= 2 (6) */
        /*  6 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /*  8 */    NdrFcLong(0x101),    /* 257 */
        /* 12 */    NdrFcShort(0x0),     /* 0 */
        /* 14 */    NdrFcShort(0x0),     /* 0 */
        /* 16 */    0xc0,       /* 192 */
        0x0,        /* 0 */
        /* 18 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 20 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 22 */    0x0,        /* 0 */
        0x46,       /* 70 */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter9_EnumServer_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IBasicEnumProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter9_EnumServer_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IBasicEnumStubVtbl,
    0
};

PCInterfaceName const _Chapter9_EnumServer_InterfaceNamesList[] = {
    "IBasicEnum",
    0
};


#define _Chapter9_EnumServer_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _Chapter9_EnumServer, pIID, n)

int __stdcall _Chapter9_EnumServer_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_Chapter9_EnumServer_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo Chapter9_EnumServer_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter9_EnumServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter9_EnumServer_StubVtblList,
    (const PCInterfaceName *) & _Chapter9_EnumServer_InterfaceNamesList,
    0, // no delegation
    & _Chapter9_EnumServer_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
