/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Dec 14 13:40:11 2000
 */
/* Compiler settings for D:\ATLSampleExe\ATLSampleExe.idl:
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


#include "ATLSampleExe.h"

#define TYPE_FORMAT_STRING_SIZE   45
#define PROC_FORMAT_STRING_SIZE   91

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
   GUID={0x6EA21C2C,0xD1C4,0x11D4,{0xA2,0x39,0x50,0x37,0x48,0xC1,0x00,0x00}} */


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
    0,
    28,
    56
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

CINTERFACE_PROXY_VTABLE(10) _IAccountProxyVtbl = {
    &IAccount_ProxyInfo,
    &IID_IAccount,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IAccount::get_Balance */ ,
    (void *) - 1 /* IAccount::put_Balance */ ,
    (void *) - 1 /* IAccount::Post */
};


static const PRPC_STUB_FUNCTION IAccount_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAccountStubVtbl = {
    &IID_IAccount,
    &IAccount_ServerInfo,
    10,
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

        /* Procedure get_Balance */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x10),    /* 16 */
        /* 14 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 16 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 22 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 24 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Balance */

        /* 28 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 30 */    NdrFcLong(0x0),      /* 0 */
        /* 34 */    NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 36 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 38 */    NdrFcShort(0x8),     /* 8 */
        /* 40 */    NdrFcShort(0x8),     /* 8 */
        /* 42 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter newVal */

        /* 44 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 46 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 48 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 50 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 52 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 54 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Post */

        /* 56 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 58 */    NdrFcLong(0x0),      /* 0 */
        /* 62 */    NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
        /* 64 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 66 */    NdrFcShort(0x8),     /* 8 */
        /* 68 */    NdrFcShort(0x8),     /* 8 */
        /* 70 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x3,        /* 3 */

        /* Parameter lAmount */

        /* 72 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 74 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 76 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter pbstrResult */

        /* 78 */    NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 80 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 82 */    NdrFcShort(0x22),    /* Type Offset=34 */

        /* Return value */

        /* 84 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 86 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 88 */    0x8,        /* FC_LONG */
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
        /*  4 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */
        /*  6 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /*  8 */    NdrFcShort(0x1a),    /* Offset= 26 (34) */
        /* 10 */
        0x13, 0x0,  /* FC_OP */
        /* 12 */    NdrFcShort(0xc),     /* Offset= 12 (24) */
        /* 14 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 16 */    NdrFcShort(0x2),     /* 2 */
        /* 18 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 20 */    NdrFcShort(0xfffc),      /* -4 */
        /* 22 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 24 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 26 */    NdrFcShort(0x8),     /* 8 */
        /* 28 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (14) */
        /* 30 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 32 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 34 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 36 */    NdrFcShort(0x0),     /* 0 */
        /* 38 */    NdrFcShort(0x4),     /* 4 */
        /* 40 */    NdrFcShort(0x0),     /* 0 */
        /* 42 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (10) */

        0x0
    }
};

const CInterfaceProxyVtbl * _ATLSampleExe_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IAccountProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ATLSampleExe_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IAccountStubVtbl,
    0
};

PCInterfaceName const _ATLSampleExe_InterfaceNamesList[] = {
    "IAccount",
    0
};

const IID *  _ATLSampleExe_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _ATLSampleExe_CHECK_IID(n)  IID_GENERIC_CHECK_IID( _ATLSampleExe, pIID, n)

int __stdcall _ATLSampleExe_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_ATLSampleExe_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo ATLSampleExe_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _ATLSampleExe_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ATLSampleExe_StubVtblList,
    (const PCInterfaceName *) & _ATLSampleExe_InterfaceNamesList,
    (const IID **) & _ATLSampleExe_BaseIIDList,
    & _ATLSampleExe_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
