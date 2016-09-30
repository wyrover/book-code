
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0347 */
/* at Sun Nov 10 13:41:53 2002
 */
/* Compiler settings for _LicenseDemo.idl:
    Os, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)

/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "_LicenseDemo.h"

#define TYPE_FORMAT_STRING_SIZE   3
#define PROC_FORMAT_STRING_SIZE   1
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


extern const MIDL_SERVER_INFO IEarPolitic_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IEarPolitic_ProxyInfo;



#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
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


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IEarPolitic, ver. 0.0,
   GUID={0x525DE96A,0xABF6,0x46DF,{0x8E,0xF5,0x7E,0x67,0x7A,0x3F,0x12,0x8C}} */

#pragma code_seg(".orpc")
static const unsigned short IEarPolitic_FormatStringOffsetTable[] = {
    0
};

static const MIDL_STUBLESS_PROXY_INFO IEarPolitic_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IEarPolitic_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO IEarPolitic_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IEarPolitic_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
const CINTERFACE_PROXY_VTABLE(3) _IEarPoliticProxyVtbl = {
    &IID_IEarPolitic,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy
};


static const PRPC_STUB_FUNCTION IEarPolitic_table[] = {
    0    /* a dummy for an empty interface */
};

const CInterfaceStubVtbl _IEarPoliticStubVtbl = {
    &IID_IEarPolitic,
    &IEarPolitic_ServerInfo,
    3,
    &IEarPolitic_table[-3],
    CStdStubBuffer_METHODS
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
    0x10001, /* Ndr library version */
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

const CInterfaceProxyVtbl * __LicenseDemo_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IEarPoliticProxyVtbl,
    0
};

const CInterfaceStubVtbl * __LicenseDemo_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IEarPoliticStubVtbl,
    0
};

PCInterfaceName const __LicenseDemo_InterfaceNamesList[] = {
    "IEarPolitic",
    0
};


#define __LicenseDemo_CHECK_IID(n)  IID_GENERIC_CHECK_IID( __LicenseDemo, pIID, n)

int __stdcall __LicenseDemo_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!__LicenseDemo_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo _LicenseDemo_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & __LicenseDemo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & __LicenseDemo_StubVtblList,
    (const PCInterfaceName *) & __LicenseDemo_InterfaceNamesList,
    0, // no delegation
    & __LicenseDemo_IID_Lookup,
    1,
    1,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

