

/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 29 16:24:45 2005
 */
/* Compiler settings for E:\Software\BookCode\VCCOM\CHAP11\DCOMSERV\DCOMServ.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
    #pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "DCOMServ.h"

#define TYPE_FORMAT_STRING_SIZE   43
#define PROC_FORMAT_STRING_SIZE   43
#define TRANSMIT_AS_TABLE_SIZE    0
#define WIRE_MARSHAL_TABLE_SIZE   1

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


extern const MIDL_SERVER_INFO IQuoteSupplier_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IQuoteSupplier_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
    #error You need a Windows 2000 or later to run this stub because it uses these features:
    #error   /robust command line switch.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure GetQuote */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
        /*  8 */    NdrFcShort(0x10),    /* x86 Stack size/offset = 16 */
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x2c),    /* 44 */
        /* 14 */    0x46,       /* Oi2 Flags:  clt must size, has return, has ext, */
        0x3,        /* 3 */
        /* 16 */    0x8,        /* 8 */
        0x5,        /* Ext Flags:  new corr desc, srv corr check, */
        /* 18 */    NdrFcShort(0x0),     /* 0 */
        /* 20 */    NdrFcShort(0x1),     /* 1 */
        /* 22 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter Ticker */

        /* 24 */    NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
        /* 26 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 28 */    NdrFcShort(0x1c),    /* Type Offset=28 */

        /* Parameter Price */

        /* 30 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 32 */    NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 34 */    0xc,        /* FC_DOUBLE */
        0x0,        /* 0 */

        /* Return value */

        /* 36 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 38 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 40 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x12, 0x0,  /* FC_UP */
        /*  4 */    NdrFcShort(0xe),     /* Offset= 14 (18) */
        /*  6 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /*  8 */    NdrFcShort(0x2),     /* 2 */
        /* 10 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 12 */    NdrFcShort(0xfffc),      /* -4 */
        /* 14 */    NdrFcShort(0x1),     /* Corr flags:  early, */
        /* 16 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 18 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 20 */    NdrFcShort(0x8),     /* 8 */
        /* 22 */    NdrFcShort(0xfff0),      /* Offset= -16 (6) */
        /* 24 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 26 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 28 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 30 */    NdrFcShort(0x0),     /* 0 */
        /* 32 */    NdrFcShort(0x4),     /* 4 */
        /* 34 */    NdrFcShort(0x0),     /* 0 */
        /* 36 */    NdrFcShort(0xffde),      /* Offset= -34 (2) */
        /* 38 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 40 */    0xc,        /* FC_DOUBLE */
        0x5c,       /* FC_PAD */

        0x0
    }
};

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = {

    {
        BSTR_UserSize
        , BSTR_UserMarshal
        , BSTR_UserUnmarshal
        , BSTR_UserFree
    }

};



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IQuoteSupplier, ver. 0.0,
   GUID={0x03A51633,0x854E,0x4942,{0x9C,0x28,0x6A,0x3C,0x94,0x35,0xB5,0x42}} */

#pragma code_seg(".orpc")
static const unsigned short IQuoteSupplier_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IQuoteSupplier_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IQuoteSupplier_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO IQuoteSupplier_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IQuoteSupplier_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(8) _IQuoteSupplierProxyVtbl = {
    &IQuoteSupplier_ProxyInfo,
    &IID_IQuoteSupplier,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* IQuoteSupplier::GetQuote */
};


static const PRPC_STUB_FUNCTION IQuoteSupplier_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IQuoteSupplierStubVtbl = {
    &IID_IQuoteSupplier,
    &IQuoteSupplier_ServerInfo,
    8,
    &IQuoteSupplier_table[-3],
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
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
};

const CInterfaceProxyVtbl * _DCOMServ_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IQuoteSupplierProxyVtbl,
    0
};

const CInterfaceStubVtbl * _DCOMServ_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IQuoteSupplierStubVtbl,
    0
};

PCInterfaceName const _DCOMServ_InterfaceNamesList[] = {
    "IQuoteSupplier",
    0
};

const IID *  _DCOMServ_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _DCOMServ_CHECK_IID(n)  IID_GENERIC_CHECK_IID( _DCOMServ, pIID, n)

int __stdcall _DCOMServ_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_DCOMServ_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo DCOMServ_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _DCOMServ_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _DCOMServ_StubVtblList,
    (const PCInterfaceName *) & _DCOMServ_InterfaceNamesList,
    (const IID **) & _DCOMServ_BaseIIDList,
    & _DCOMServ_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
    #pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 29 16:24:45 2005
 */
/* Compiler settings for E:\Software\BookCode\VCCOM\CHAP11\DCOMSERV\DCOMServ.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
    #pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "DCOMServ.h"

#define TYPE_FORMAT_STRING_SIZE   43
#define PROC_FORMAT_STRING_SIZE   45
#define TRANSMIT_AS_TABLE_SIZE    0
#define WIRE_MARSHAL_TABLE_SIZE   1

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


extern const MIDL_SERVER_INFO IQuoteSupplier_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IQuoteSupplier_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
    #error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure GetQuote */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
        /*  8 */    NdrFcShort(0x20),    /* ia64 Stack size/offset = 32 */
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x2c),    /* 44 */
        /* 14 */    0x46,       /* Oi2 Flags:  clt must size, has return, has ext, */
        0x3,        /* 3 */
        /* 16 */    0xa,        /* 10 */
        0x5,        /* Ext Flags:  new corr desc, srv corr check, */
        /* 18 */    NdrFcShort(0x0),     /* 0 */
        /* 20 */    NdrFcShort(0x1),     /* 1 */
        /* 22 */    NdrFcShort(0x0),     /* 0 */
        /* 24 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter Ticker */

        /* 26 */    NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
        /* 28 */    NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 30 */    NdrFcShort(0x1c),    /* Type Offset=28 */

        /* Parameter Price */

        /* 32 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 34 */    NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 36 */    0xc,        /* FC_DOUBLE */
        0x0,        /* 0 */

        /* Return value */

        /* 38 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 40 */    NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 42 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x12, 0x0,  /* FC_UP */
        /*  4 */    NdrFcShort(0xe),     /* Offset= 14 (18) */
        /*  6 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /*  8 */    NdrFcShort(0x2),     /* 2 */
        /* 10 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 12 */    NdrFcShort(0xfffc),      /* -4 */
        /* 14 */    NdrFcShort(0x1),     /* Corr flags:  early, */
        /* 16 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 18 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 20 */    NdrFcShort(0x8),     /* 8 */
        /* 22 */    NdrFcShort(0xfff0),      /* Offset= -16 (6) */
        /* 24 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 26 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 28 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 30 */    NdrFcShort(0x0),     /* 0 */
        /* 32 */    NdrFcShort(0x8),     /* 8 */
        /* 34 */    NdrFcShort(0x0),     /* 0 */
        /* 36 */    NdrFcShort(0xffde),      /* Offset= -34 (2) */
        /* 38 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 40 */    0xc,        /* FC_DOUBLE */
        0x5c,       /* FC_PAD */

        0x0
    }
};

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = {

    {
        BSTR_UserSize
        , BSTR_UserMarshal
        , BSTR_UserUnmarshal
        , BSTR_UserFree
    }

};



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IQuoteSupplier, ver. 0.0,
   GUID={0x03A51633,0x854E,0x4942,{0x9C,0x28,0x6A,0x3C,0x94,0x35,0xB5,0x42}} */

#pragma code_seg(".orpc")
static const unsigned short IQuoteSupplier_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IQuoteSupplier_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IQuoteSupplier_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO IQuoteSupplier_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IQuoteSupplier_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(8) _IQuoteSupplierProxyVtbl = {
    &IQuoteSupplier_ProxyInfo,
    &IID_IQuoteSupplier,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* IQuoteSupplier::GetQuote */
};


static const PRPC_STUB_FUNCTION IQuoteSupplier_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IQuoteSupplierStubVtbl = {
    &IID_IQuoteSupplier,
    &IQuoteSupplier_ServerInfo,
    8,
    &IQuoteSupplier_table[-3],
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
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
};

const CInterfaceProxyVtbl * _DCOMServ_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IQuoteSupplierProxyVtbl,
    0
};

const CInterfaceStubVtbl * _DCOMServ_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IQuoteSupplierStubVtbl,
    0
};

PCInterfaceName const _DCOMServ_InterfaceNamesList[] = {
    "IQuoteSupplier",
    0
};

const IID *  _DCOMServ_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _DCOMServ_CHECK_IID(n)  IID_GENERIC_CHECK_IID( _DCOMServ, pIID, n)

int __stdcall _DCOMServ_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_DCOMServ_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo DCOMServ_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _DCOMServ_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _DCOMServ_StubVtblList,
    (const PCInterfaceName *) & _DCOMServ_InterfaceNamesList,
    (const IID **) & _DCOMServ_BaseIIDList,
    & _DCOMServ_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
    #pragma warning(pop)
#endif


#endif /* defined(_M_IA64) || defined(_M_AMD64)*/

