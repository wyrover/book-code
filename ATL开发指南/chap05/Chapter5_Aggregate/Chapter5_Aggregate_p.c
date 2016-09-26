/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 11 10:51:37 1998
 */
/* Compiler settings for Chapter5_Aggregate.idl:
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


#include "Chapter5_Aggregate.h"

#define TYPE_FORMAT_STRING_SIZE   7
#define PROC_FORMAT_STRING_SIZE   69

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


/* Object interface: IAdvancedMath, ver. 0.0,
   GUID={0x380F79F9,0x4098,0x11D1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdvancedMath_ServerInfo;

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

static const unsigned short IAdvancedMath_FormatStringOffsetTable[] = {
    0,
    34
};

static const MIDL_SERVER_INFO IAdvancedMath_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdvancedMath_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IAdvancedMath_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdvancedMath_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(5) _IAdvancedMathProxyVtbl = {
    &IAdvancedMath_ProxyInfo,
    &IID_IAdvancedMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IAdvancedMath::Factorial */ ,
    (void *) - 1 /* IAdvancedMath::Fibonacci */
};

const CInterfaceStubVtbl _IAdvancedMathStubVtbl = {
    &IID_IAdvancedMath,
    &IAdvancedMath_ServerInfo,
    5,
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

        /* Procedure Factorial */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 10 */    NdrFcShort(0xe),     /* 14 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x4,        /* Oi2 Flags:  has return, */
        0x3,        /* 3 */

        /* Parameter sOp */

        /* 16 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 22 */    NdrFcShort(0x148),   /* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
        /* 24 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 28 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 30 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 32 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Fibonacci */

        /* 34 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 36 */    NdrFcLong(0x0),      /* 0 */
        /* 40 */    NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 42 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 44 */    NdrFcShort(0xe),     /* 14 */
        /* 46 */    NdrFcShort(0x8),     /* 8 */
        /* 48 */    0x4,        /* Oi2 Flags:  has return, */
        0x3,        /* 3 */

        /* Parameter sOp */

        /* 50 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 52 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 54 */    0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 56 */    NdrFcShort(0x148),   /* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
        /* 58 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 60 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 62 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 64 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 66 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x11, 0x8,  /* FC_RP [simple_pointer] */
        /*  4 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter5_Aggregate_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IAdvancedMathProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter5_Aggregate_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IAdvancedMathStubVtbl,
    0
};

PCInterfaceName const _Chapter5_Aggregate_InterfaceNamesList[] = {
    "IAdvancedMath",
    0
};


#define _Chapter5_Aggregate_CHECK_IID(n)    IID_GENERIC_CHECK_IID( _Chapter5_Aggregate, pIID, n)

int __stdcall _Chapter5_Aggregate_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_Chapter5_Aggregate_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo Chapter5_Aggregate_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter5_Aggregate_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter5_Aggregate_StubVtblList,
    (const PCInterfaceName *) & _Chapter5_Aggregate_InterfaceNamesList,
    0, // no delegation
    & _Chapter5_Aggregate_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
