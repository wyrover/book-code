/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 3.01.75 */
/* at Fri Mar 20 12:55:54 1998
 */
/* Compiler settings for Chapter10_GITServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY

#include "rpcproxy.h"
#include "Chapter10_GITServer.h"

#define TYPE_FORMAT_STRING_SIZE   53
#define PROC_FORMAT_STRING_SIZE   211

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


/* Object interface: ICallBack, ver. 0.0,
   GUID={0x9029D3B0,0x67FE,0x11d1,{0xB5,0xF9,0x00,0x04,0xAC,0xFF,0x17,0x1C}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICallBack_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short ICallBack_FormatStringOffsetTable[] = {
    0,
    24
};

static const MIDL_SERVER_INFO ICallBack_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ICallBack_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO ICallBack_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ICallBack_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(5) _ICallBackProxyVtbl = {
    &ICallBack_ProxyInfo,
    &IID_ICallBack,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* ICallBack::ComputationComplete */ ,
    (void *) - 1 /* ICallBack::Error */
};

const CInterfaceStubVtbl _ICallBackStubVtbl = {
    &IID_ICallBack,
    &ICallBack_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IMath, ver. 0.0,
   GUID={0xC2C66744,0xC012,0x11D1,{0xB6,0x19,0x00,0x04,0xAC,0xFF,0x17,0x1C}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMath_ServerInfo;

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
    0, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x301004b, /* MIDL Version 3.1.75 */
    0,
    UserMarshalRoutines,
    0,  /* Reserved1 */
    0,  /* Reserved2 */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

static const unsigned short IMath_FormatStringOffsetTable[] = {
    48,
    78,
    108,
    138,
    168,
    192
};

static const MIDL_SERVER_INFO IMath_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMath_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IMath_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMath_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(9) _IMathProxyVtbl = {
    &IMath_ProxyInfo,
    &IID_IMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IMath::Add */ ,
    (void *) - 1 /* IMath::Subtract */ ,
    (void *) - 1 /* IMath::Multiply */ ,
    (void *) - 1 /* IMath::Divide */ ,
    (void *) - 1 /* IMath::Advise */ ,
    (void *) - 1 /* IMath::Unadvise */
};

const CInterfaceStubVtbl _IMathStubVtbl = {
    &IID_IMath,
    &IMath_ServerInfo,
    9,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
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
    #error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure ComputationComplete */

        0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /*  2 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /*  4 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /*  6 */    NdrFcShort(0x8),     /* 8 */
        /*  8 */    NdrFcShort(0x8),     /* 8 */
        /* 10 */    0x4,        /* 4 */
        0x2,        /* 2 */

        /* Parameter lResult */

        /* 12 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 14 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 16 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 18 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 20 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 22 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Error */

        /* 24 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 26 */    NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 28 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 30 */    NdrFcShort(0x0),     /* 0 */
        /* 32 */    NdrFcShort(0x8),     /* 8 */
        /* 34 */    0x6,        /* 6 */
        0x2,        /* 2 */

        /* Parameter bstrDescription */

        /* 36 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
        /* 38 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 40 */    NdrFcShort(0x18),    /* Type Offset=24 */

        /* Return value */

        /* 42 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 44 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 46 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Add */

        /* 48 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 50 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /* 52 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 54 */    NdrFcShort(0x10),    /* 16 */
        /* 56 */    NdrFcShort(0x8),     /* 8 */
        /* 58 */    0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 60 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 62 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 64 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 66 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 68 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 70 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 72 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 74 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 76 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Subtract */

        /* 78 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 80 */    NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 82 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 84 */    NdrFcShort(0x10),    /* 16 */
        /* 86 */    NdrFcShort(0x8),     /* 8 */
        /* 88 */    0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 90 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 92 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 94 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 96 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 98 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 100 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 102 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 104 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 106 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Multiply */

        /* 108 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 110 */   NdrFcShort(0x5),     /* 5 */
#ifndef _ALPHA_
        /* 112 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 114 */   NdrFcShort(0x10),    /* 16 */
        /* 116 */   NdrFcShort(0x8),     /* 8 */
        /* 118 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 120 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 122 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 124 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 126 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 128 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 130 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 132 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 134 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 136 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Divide */

        /* 138 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 140 */   NdrFcShort(0x6),     /* 6 */
#ifndef _ALPHA_
        /* 142 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 144 */   NdrFcShort(0x10),    /* 16 */
        /* 146 */   NdrFcShort(0x8),     /* 8 */
        /* 148 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 150 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 152 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 154 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 156 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 158 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 160 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 162 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 164 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 166 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Advise */

        /* 168 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 170 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 172 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 174 */   NdrFcShort(0x0),     /* 0 */
        /* 176 */   NdrFcShort(0x8),     /* 8 */
        /* 178 */   0x6,        /* 6 */
        0x2,        /* 2 */

        /* Parameter pCallBack */

        /* 180 */   NdrFcShort(0xb),     /* 11 */
#ifndef _ALPHA_
        /* 182 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 184 */   NdrFcShort(0x22),    /* Type Offset=34 */

        /* Return value */

        /* 186 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 188 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 190 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Unadvise */

        /* 192 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 194 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 196 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 198 */   NdrFcShort(0x0),     /* 0 */
        /* 200 */   NdrFcShort(0x8),     /* 8 */
        /* 202 */   0x4,        /* 4 */
        0x1,        /* 1 */

        /* Return value */

        /* 204 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 206 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 208 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        0x12, 0x0,  /* FC_UP */
        /*  2 */    NdrFcShort(0xc),     /* Offset= 12 (14) */
        /*  4 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /*  6 */    NdrFcShort(0x2),     /* 2 */
        /*  8 */    0x9,        /* 9 */
        0x0,        /*  */
        /* 10 */    NdrFcShort(0xfffffffc),      /* -4 */
        /* 12 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 14 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 16 */    NdrFcShort(0x8),     /* 8 */
        /* 18 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (4) */
        /* 20 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 22 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 24 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 26 */    NdrFcShort(0x0),     /* 0 */
        /* 28 */    NdrFcShort(0x4),     /* 4 */
        /* 30 */    NdrFcShort(0x0),     /* 0 */
        /* 32 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (0) */
        /* 34 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 36 */    NdrFcLong(0x9029d3b0),   /* -1876307024 */
        /* 40 */    NdrFcShort(0x67fe),      /* 26622 */
        /* 42 */    NdrFcShort(0x11d1),      /* 4561 */
        /* 44 */    0xb5,       /* 181 */
        0xf9,       /* 249 */
        /* 46 */    0x0,        /* 0 */
        0x4,        /* 4 */
        /* 48 */    0xac,       /* 172 */
        0xff,       /* 255 */
        /* 50 */    0x17,       /* 23 */
        0x1c,       /* 28 */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter10_GITServer_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IMathProxyVtbl,
    (CInterfaceProxyVtbl *) &_ICallBackProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter10_GITServer_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IMathStubVtbl,
    (CInterfaceStubVtbl *) &_ICallBackStubVtbl,
    0
};

PCInterfaceName const _Chapter10_GITServer_InterfaceNamesList[] = {
    "IMath",
    "ICallBack",
    0
};


#define _Chapter10_GITServer_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _Chapter10_GITServer, pIID, n)

int __stdcall _Chapter10_GITServer_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_Chapter10_GITServer, 2, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_Chapter10_GITServer, 2, *pIndex)
}

const ExtendedProxyFileInfo Chapter10_GITServer_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter10_GITServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter10_GITServer_StubVtblList,
    (const PCInterfaceName *) & _Chapter10_GITServer_InterfaceNamesList,
    0, // no delegation
    & _Chapter10_GITServer_IID_Lookup,
    2,
    2
};
