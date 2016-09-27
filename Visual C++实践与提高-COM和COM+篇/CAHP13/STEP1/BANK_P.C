/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Aug 26 23:15:48 2000
 */
/* Compiler settings for D:\Samples\Bank\Bank.idl:
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


#include "Bank.h"

#define TYPE_FORMAT_STRING_SIZE   41
#define PROC_FORMAT_STRING_SIZE   41

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


/* Object interface: IAccount, ver. 0.0,
   GUID={0x5939F230,0x7B5F,0x11D4,{0x8B,0xEC,0x34,0x8F,0xBB,0x00,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAccount_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1];

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
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

static const unsigned short IAccount_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0
};

static const MIDL_SERVER_INFO IAccount_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAccount_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IAccount_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAccount_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(8) _IAccountProxyVtbl = {
    &IAccount_ProxyInfo,
    &IID_IAccount,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IAccount::Post */
};


static const PRPC_STUB_FUNCTION IAccount_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IAccountStubVtbl = {
    &IID_IAccount,
    &IAccount_ServerInfo,
    8,
    &IAccount_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1] = {

    {
        BSTR_UserSize
        , BSTR_UserMarshal
        , BSTR_UserUnmarshal
        , BSTR_UserFree
    }

};


#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
    #error You need a Windows NT 4.0 or later to run this stub because it uses these features:
    #error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure Post */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 10 */    NdrFcShort(0x10),    /* 16 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x4,        /* 4 */

        /* Parameter lAccount */

        /* 16 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lAmount */

        /* 22 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 24 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter pbstrResult */

        /* 28 */    NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 30 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 32 */    NdrFcShort(0x1e),    /* Type Offset=30 */

        /* Return value */

        /* 34 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 36 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
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
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /*  4 */    NdrFcShort(0x1a),    /* Offset= 26 (30) */
        /*  6 */
        0x13, 0x0,  /* FC_OP */
        /*  8 */    NdrFcShort(0xc),     /* Offset= 12 (20) */
        /* 10 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 12 */    NdrFcShort(0x2),     /* 2 */
        /* 14 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 16 */    NdrFcShort(0xfffc),      /* -4 */
        /* 18 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 20 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 22 */    NdrFcShort(0x8),     /* 8 */
        /* 24 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (10) */
        /* 26 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 28 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 30 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 32 */    NdrFcShort(0x0),     /* 0 */
        /* 34 */    NdrFcShort(0x4),     /* 4 */
        /* 36 */    NdrFcShort(0x0),     /* 0 */
        /* 38 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (6) */

        0x0
    }
};

const CInterfaceProxyVtbl * _Bank_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IAccountProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Bank_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IAccountStubVtbl,
    0
};

PCInterfaceName const _Bank_InterfaceNamesList[] = {
    "IAccount",
    0
};

const IID *  _Bank_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _Bank_CHECK_IID(n)  IID_GENERIC_CHECK_IID( _Bank, pIID, n)

int __stdcall _Bank_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_Bank_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo Bank_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Bank_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Bank_StubVtblList,
    (const PCInterfaceName *) & _Bank_InterfaceNamesList,
    (const IID **) & _Bank_BaseIIDList,
    & _Bank_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
