/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 28 10:45:33 2000
 */
/* Compiler settings for C:\VCCOM\chap10\DOMSamp\DOMSamp.idl:
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


#include "DOMSamp.h"

#define TYPE_FORMAT_STRING_SIZE   3
#define PROC_FORMAT_STRING_SIZE   1

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


/* Object interface: IDOMCtrl, ver. 0.0,
   GUID={0x63D6A83B,0x94C4,0x11D4,{0x89,0x6A,0x80,0xFF,0x10,0x0A,0xAD,0x79}} */


extern const MIDL_STUB_DESC Object_StubDesc;


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

CINTERFACE_PROXY_VTABLE(7) _IDOMCtrlProxyVtbl = {
    0,
    &IID_IDOMCtrl,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION IDOMCtrl_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IDOMCtrlStubVtbl = {
    &IID_IDOMCtrl,
    0,
    7,
    &IDOMCtrl_table[-3],
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

const CInterfaceProxyVtbl * _DOMSamp_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IDOMCtrlProxyVtbl,
    0
};

const CInterfaceStubVtbl * _DOMSamp_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IDOMCtrlStubVtbl,
    0
};

PCInterfaceName const _DOMSamp_InterfaceNamesList[] = {
    "IDOMCtrl",
    0
};

const IID *  _DOMSamp_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _DOMSamp_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _DOMSamp, pIID, n)

int __stdcall _DOMSamp_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_DOMSamp_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo DOMSamp_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _DOMSamp_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _DOMSamp_StubVtblList,
    (const PCInterfaceName *) & _DOMSamp_InterfaceNamesList,
    (const IID **) & _DOMSamp_BaseIIDList,
    & _DOMSamp_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
