
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0347 */
/* at Mon Sep 23 10:15:08 2002
 */
/* Compiler settings for Math.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "Math.h"

#define TYPE_FORMAT_STRING_SIZE   7
#define PROC_FORMAT_STRING_SIZE   41
#define TRANSMIT_AS_TABLE_SIZE    0
#define WIRE_MARSHAL_TABLE_SIZE   0

typedef struct _MIDL_TYPE_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
} MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax =
{{0x8A885D04, 0x1CEB, 0x11C9, {0x9F, 0xE8, 0x08, 0x00, 0x2B, 0x10, 0x48, 0x60}}, {2, 0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICalc_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICalc_ProxyInfo;



#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
    #error You need a Windows NT 4.0 or later to run this stub because it uses these features:
    #error   -Oif or -Oicf, float, double or hyper in -Oif or -Oicf.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure Add */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
        /*  8 */    NdrFcShort(0x1c),    /* x86 Stack size/offset = 28 */
        /* 10 */    NdrFcShort(0x20),    /* 32 */
        /* 12 */    NdrFcShort(0x2c),    /* 44 */
        /* 14 */    0x4,        /* Oi2 Flags:  has return, */
        0x4,        /* 4 */

        /* Parameter Op1 */

        /* 16 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 18 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 20 */    0xc,        /* FC_DOUBLE */
        0x0,        /* 0 */

        /* Parameter Op2 */

        /* 22 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 24 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 26 */    0xc,        /* FC_DOUBLE */
        0x0,        /* 0 */

        /* Parameter Result */

        /* 28 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 30 */    NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
        /* 32 */    0xc,        /* FC_DOUBLE */
        0x0,        /* 0 */

        /* Return value */

        /* 34 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 36 */    NdrFcShort(0x18),    /* x86 Stack size/offset = 24 */
        /* 38 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /*  4 */    0xc,        /* FC_DOUBLE */
        0x5c,       /* FC_PAD */

        0x0
    }
};


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ICalc, ver. 0.0,
   GUID={0x2F6C88D7,0xC2BF,0x4933,{0x81,0xFA,0x3F,0xBA,0xFC,0x3F,0xC3,0x4B}} */

#pragma code_seg(".orpc")
static const unsigned short ICalc_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0
};

static const MIDL_STUBLESS_PROXY_INFO ICalc_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ICalc_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO ICalc_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ICalc_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(8) _ICalcProxyVtbl = {
    &ICalc_ProxyInfo,
    &IID_ICalc,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* ICalc::Add */
};


static const PRPC_STUB_FUNCTION ICalc_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _ICalcStubVtbl = {
    &IID_ICalc,
    &ICalc_ServerInfo,
    8,
    &ICalc_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

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
    0x600015b, /* MIDL Version 6.0.347 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
};

const CInterfaceProxyVtbl * _Math_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_ICalcProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Math_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_ICalcStubVtbl,
    0
};

PCInterfaceName const _Math_InterfaceNamesList[] = {
    "ICalc",
    0
};

const IID *  _Math_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _Math_CHECK_IID(n)  IID_GENERIC_CHECK_IID( _Math, pIID, n)

int __stdcall _Math_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_Math_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo Math_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Math_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Math_StubVtblList,
    (const PCInterfaceName *) & _Math_InterfaceNamesList,
    (const IID **) & _Math_BaseIIDList,
    & _Math_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

