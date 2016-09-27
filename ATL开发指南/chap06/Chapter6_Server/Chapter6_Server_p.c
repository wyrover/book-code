/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 3.01.75 */
/* at Wed Oct 29 19:50:22 1997
 */
/* Compiler settings for Chapter6_Server.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY

#include "rpcproxy.h"
#include "Chapter6_Server.h"

#define TYPE_FORMAT_STRING_SIZE   1093
#define PROC_FORMAT_STRING_SIZE   379

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


/* Object interface: IMath, ver. 0.0,
   GUID={0xDCA4F88E,0x4952,0x11D1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMath_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IMath_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    36,
    72,
    108
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

CINTERFACE_PROXY_VTABLE(11) _IMathProxyVtbl = {
    &IMath_ProxyInfo,
    &IID_IMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IMath::Add */ ,
    (void *) - 1 /* IMath::Subtract */ ,
    (void *) - 1 /* IMath::Multiply */ ,
    (void *) - 1 /* IMath::Divide */
};


static const PRPC_STUB_FUNCTION IMath_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMathStubVtbl = {
    &IID_IMath,
    &IMath_ServerInfo,
    11,
    &IMath_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMath2, ver. 0.0,
   GUID={0x9F21BD41,0x4E25,0x11d1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMath2_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IMath2_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    144,
    180,
    216,
    252,
    288
};

static const MIDL_SERVER_INFO IMath2_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMath2_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IMath2_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMath2_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(12) _IMath2ProxyVtbl = {
    &IMath2_ProxyInfo,
    &IID_IMath2,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IMath2::Add */ ,
    (void *) - 1 /* IMath2::Subtract */ ,
    (void *) - 1 /* IMath2::Multiply */ ,
    (void *) - 1 /* IMath2::Divide */ ,
    (void *) - 1 /* IMath2::Sum */
};


static const PRPC_STUB_FUNCTION IMath2_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMath2StubVtbl = {
    &IID_IMath2,
    &IMath2_ServerInfo,
    12,
    &IMath2_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAdvancedMath, ver. 0.0,
   GUID={0x9F21BD45,0x4E25,0x11d1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdvancedMath_ServerInfo;

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

static const unsigned short IAdvancedMath_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    318,
    348
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

CINTERFACE_PROXY_VTABLE(9) _IAdvancedMathProxyVtbl = {
    &IAdvancedMath_ProxyInfo,
    &IID_IAdvancedMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IAdvancedMath::Factorial */ ,
    (void *) - 1 /* IAdvancedMath::Fibonacci */
};


static const PRPC_STUB_FUNCTION IAdvancedMath_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdvancedMathStubVtbl = {
    &IID_IAdvancedMath,
    &IAdvancedMath_ServerInfo,
    9,
    &IAdvancedMath_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1] = {

    {
        VARIANT_UserSize
        , VARIANT_UserMarshal
        , VARIANT_UserUnmarshal
        , VARIANT_UserFree
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

        /* Procedure Add */

        0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /*  2 */    NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /*  4 */    NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /*  6 */    NdrFcShort(0x0),     /* 0 */
        /*  8 */    NdrFcShort(0x8),     /* 8 */
        /* 10 */    0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 12 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 14 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 16 */    NdrFcShort(0x322),   /* Type Offset=802 */

        /* Parameter varOp2 */

        /* 18 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 20 */    NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 22 */    NdrFcShort(0x32c),   /* Type Offset=812 */

        /* Parameter pvarResult */

        /* 24 */    NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 26 */    NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 28 */    NdrFcShort(0x33e),   /* Type Offset=830 */

        /* Return value */

        /* 30 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 32 */    NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 34 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Subtract */

        /* 36 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 38 */    NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 40 */    NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 42 */    NdrFcShort(0x0),     /* 0 */
        /* 44 */    NdrFcShort(0x8),     /* 8 */
        /* 46 */    0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 48 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 50 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 52 */    NdrFcShort(0x348),   /* Type Offset=840 */

        /* Parameter varOp2 */

        /* 54 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 56 */    NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 58 */    NdrFcShort(0x352),   /* Type Offset=850 */

        /* Parameter pvarResult */

        /* 60 */    NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 62 */    NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 64 */    NdrFcShort(0x360),   /* Type Offset=864 */

        /* Return value */

        /* 66 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 68 */    NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 70 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Multiply */

        /* 72 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 74 */    NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 76 */    NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 78 */    NdrFcShort(0x0),     /* 0 */
        /* 80 */    NdrFcShort(0x8),     /* 8 */
        /* 82 */    0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 84 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 86 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 88 */    NdrFcShort(0x36a),   /* Type Offset=874 */

        /* Parameter varOp2 */

        /* 90 */    NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 92 */    NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 94 */    NdrFcShort(0x374),   /* Type Offset=884 */

        /* Parameter pvarResult */

        /* 96 */    NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 98 */    NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 100 */   NdrFcShort(0x382),   /* Type Offset=898 */

        /* Return value */

        /* 102 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 104 */   NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 106 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Divide */

        /* 108 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 110 */   NdrFcShort(0xa),     /* 10 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 112 */   NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 114 */   NdrFcShort(0x0),     /* 0 */
        /* 116 */   NdrFcShort(0x8),     /* 8 */
        /* 118 */   0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 120 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 122 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 124 */   NdrFcShort(0x38c),   /* Type Offset=908 */

        /* Parameter varOp2 */

        /* 126 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 128 */   NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 130 */   NdrFcShort(0x396),   /* Type Offset=918 */

        /* Parameter pvarResult */

        /* 132 */   NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 134 */   NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 136 */   NdrFcShort(0x3a4),   /* Type Offset=932 */

        /* Return value */

        /* 138 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 140 */   NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 142 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Add */

        /* 144 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 146 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 148 */   NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 150 */   NdrFcShort(0x0),     /* 0 */
        /* 152 */   NdrFcShort(0x8),     /* 8 */
        /* 154 */   0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 156 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 158 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 160 */   NdrFcShort(0x3ae),   /* Type Offset=942 */

        /* Parameter varOp2 */

        /* 162 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 164 */   NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 166 */   NdrFcShort(0x3b8),   /* Type Offset=952 */

        /* Parameter pvarResult */

        /* 168 */   NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 170 */   NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 172 */   NdrFcShort(0x3c6),   /* Type Offset=966 */

        /* Return value */

        /* 174 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 176 */   NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 178 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Subtract */

        /* 180 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 182 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 184 */   NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 186 */   NdrFcShort(0x0),     /* 0 */
        /* 188 */   NdrFcShort(0x8),     /* 8 */
        /* 190 */   0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 192 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 194 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 196 */   NdrFcShort(0x3d0),   /* Type Offset=976 */

        /* Parameter varOp2 */

        /* 198 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 200 */   NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 202 */   NdrFcShort(0x3da),   /* Type Offset=986 */

        /* Parameter pvarResult */

        /* 204 */   NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 206 */   NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 208 */   NdrFcShort(0x3e8),   /* Type Offset=1000 */

        /* Return value */

        /* 210 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 212 */   NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 214 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Multiply */

        /* 216 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 218 */   NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 220 */   NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 222 */   NdrFcShort(0x0),     /* 0 */
        /* 224 */   NdrFcShort(0x8),     /* 8 */
        /* 226 */   0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 228 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 230 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 232 */   NdrFcShort(0x3f2),   /* Type Offset=1010 */

        /* Parameter varOp2 */

        /* 234 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 236 */   NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 238 */   NdrFcShort(0x3fc),   /* Type Offset=1020 */

        /* Parameter pvarResult */

        /* 240 */   NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 242 */   NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 244 */   NdrFcShort(0x40a),   /* Type Offset=1034 */

        /* Return value */

        /* 246 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 248 */   NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 250 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Divide */

        /* 252 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 254 */   NdrFcShort(0xa),     /* 10 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 256 */   NdrFcShort(0x2c),    /* x86 Stack size/offset = 44 */
#else
        NdrFcShort(0x30),    /* MIPS & PPC Stack size/offset = 48 */
#endif
#else
        NdrFcShort(0x38),    /* Alpha Stack size/offset = 56 */
#endif
        /* 258 */   NdrFcShort(0x0),     /* 0 */
        /* 260 */   NdrFcShort(0x8),     /* 8 */
        /* 262 */   0x7,        /* 7 */
        0x4,        /* 4 */

        /* Parameter varOp1 */

        /* 264 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 266 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 268 */   NdrFcShort(0x414),   /* Type Offset=1044 */

        /* Parameter varOp2 */

        /* 270 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 272 */   NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 274 */   NdrFcShort(0x41e),   /* Type Offset=1054 */

        /* Parameter pvarResult */

        /* 276 */   NdrFcShort(0x4113),      /* 16659 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 278 */   NdrFcShort(0x24),    /* x86 Stack size/offset = 36 */
#else
        NdrFcShort(0x28),    /* MIPS & PPC Stack size/offset = 40 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 280 */   NdrFcShort(0x42c),   /* Type Offset=1068 */

        /* Return value */

        /* 282 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 284 */   NdrFcShort(0x28),    /* x86 Stack size/offset = 40 */
#else
        NdrFcShort(0x2c),    /* MIPS & PPC Stack size/offset = 44 */
#endif
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 286 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Sum */

        /* 288 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 290 */   NdrFcShort(0xb),     /* 11 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 292 */   NdrFcShort(0x1c),    /* x86 Stack size/offset = 28 */
#else
        NdrFcShort(0x20),    /* MIPS & PPC Stack size/offset = 32 */
#endif
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 294 */   NdrFcShort(0x0),     /* 0 */
        /* 296 */   NdrFcShort(0x10),    /* 16 */
        /* 298 */   0x6,        /* 6 */
        0x3,        /* 3 */

        /* Parameter varOp1 */

        /* 300 */   NdrFcShort(0x8b),    /* 139 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 302 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* MIPS & PPC Stack size/offset = 8 */
#endif
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 304 */   NdrFcShort(0x436),   /* Type Offset=1078 */

        /* Parameter plResult */

        /* 306 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 308 */   NdrFcShort(0x14),    /* x86 Stack size/offset = 20 */
#else
        NdrFcShort(0x18),    /* MIPS & PPC Stack size/offset = 24 */
#endif
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 310 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 312 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
        /* 314 */   NdrFcShort(0x18),    /* x86 Stack size/offset = 24 */
#else
        NdrFcShort(0x1c),    /* MIPS & PPC Stack size/offset = 28 */
#endif
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 316 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Factorial */

        /* 318 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 320 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 322 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 324 */   NdrFcShort(0x6),     /* 6 */
        /* 326 */   NdrFcShort(0x10),    /* 16 */
        /* 328 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter sOp */

        /* 330 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 332 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 334 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 336 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 338 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 340 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 342 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 344 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 346 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Fibonacci */

        /* 348 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 350 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 352 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 354 */   NdrFcShort(0x6),     /* 6 */
        /* 356 */   NdrFcShort(0x10),    /* 16 */
        /* 358 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter sOp */

        /* 360 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 362 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 364 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 366 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 368 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 370 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 372 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 374 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 376 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        0x12, 0x0,  /* FC_UP */
        /*  2 */    NdrFcShort(0x30c),   /* Offset= 780 (782) */
        /*  4 */
        0x2b,       /* FC_NON_ENCAPSULATED_UNION */
        0x6,        /* FC_SHORT */
        /*  6 */    0x6,        /* 6 */
        0x0,        /*  */
        /*  8 */    NdrFcShort(0xfffffff8),      /* -8 */
        /* 10 */    NdrFcShort(0x2),     /* Offset= 2 (12) */
        /* 12 */    NdrFcShort(0x10),    /* 16 */
        /* 14 */    NdrFcShort(0x29),    /* 41 */
        /* 16 */    NdrFcLong(0x3),      /* 3 */
        /* 20 */    NdrFcShort(0xffff8008),      /* Offset= -32760 (-32740) */
        /* 22 */    NdrFcLong(0x11),     /* 17 */
        /* 26 */    NdrFcShort(0xffff8002),      /* Offset= -32766 (-32740) */
        /* 28 */    NdrFcLong(0x2),      /* 2 */
        /* 32 */    NdrFcShort(0xffff8006),      /* Offset= -32762 (-32730) */
        /* 34 */    NdrFcLong(0x4),      /* 4 */
        /* 38 */    NdrFcShort(0xffff800a),      /* Offset= -32758 (-32720) */
        /* 40 */    NdrFcLong(0x5),      /* 5 */
        /* 44 */    NdrFcShort(0xffff800c),      /* Offset= -32756 (-32712) */
        /* 46 */    NdrFcLong(0xb),      /* 11 */
        /* 50 */    NdrFcShort(0xffff8006),      /* Offset= -32762 (-32712) */
        /* 52 */    NdrFcLong(0xa),      /* 10 */
        /* 56 */    NdrFcShort(0xffff8008),      /* Offset= -32760 (-32704) */
        /* 58 */    NdrFcLong(0x6),      /* 6 */
        /* 62 */    NdrFcShort(0xca),    /* Offset= 202 (264) */
        /* 64 */    NdrFcLong(0x7),      /* 7 */
        /* 68 */    NdrFcShort(0xffff800c),      /* Offset= -32756 (-32688) */
        /* 70 */    NdrFcLong(0x8),      /* 8 */
        /* 74 */    NdrFcShort(0xc4),    /* Offset= 196 (270) */
        /* 76 */    NdrFcLong(0xd),      /* 13 */
        /* 80 */    NdrFcShort(0xd6),    /* Offset= 214 (294) */
        /* 82 */    NdrFcLong(0x9),      /* 9 */
        /* 86 */    NdrFcShort(0xe2),    /* Offset= 226 (312) */
        /* 88 */    NdrFcLong(0x2000),   /* 8192 */
        /* 92 */    NdrFcShort(0xee),    /* Offset= 238 (330) */
        /* 94 */    NdrFcLong(0x4011),   /* 16401 */
        /* 98 */    NdrFcShort(0x26c),   /* Offset= 620 (718) */
        /* 100 */   NdrFcLong(0x4002),   /* 16386 */
        /* 104 */   NdrFcShort(0x26a),   /* Offset= 618 (722) */
        /* 106 */   NdrFcLong(0x4003),   /* 16387 */
        /* 110 */   NdrFcShort(0x268),   /* Offset= 616 (726) */
        /* 112 */   NdrFcLong(0x4004),   /* 16388 */
        /* 116 */   NdrFcShort(0x266),   /* Offset= 614 (730) */
        /* 118 */   NdrFcLong(0x4005),   /* 16389 */
        /* 122 */   NdrFcShort(0x264),   /* Offset= 612 (734) */
        /* 124 */   NdrFcLong(0x400b),   /* 16395 */
        /* 128 */   NdrFcShort(0x252),   /* Offset= 594 (722) */
        /* 130 */   NdrFcLong(0x400a),   /* 16394 */
        /* 134 */   NdrFcShort(0x250),   /* Offset= 592 (726) */
        /* 136 */   NdrFcLong(0x4006),   /* 16390 */
        /* 140 */   NdrFcShort(0x256),   /* Offset= 598 (738) */
        /* 142 */   NdrFcLong(0x4007),   /* 16391 */
        /* 146 */   NdrFcShort(0x24c),   /* Offset= 588 (734) */
        /* 148 */   NdrFcLong(0x4008),   /* 16392 */
        /* 152 */   NdrFcShort(0x24e),   /* Offset= 590 (742) */
        /* 154 */   NdrFcLong(0x400d),   /* 16397 */
        /* 158 */   NdrFcShort(0x24c),   /* Offset= 588 (746) */
        /* 160 */   NdrFcLong(0x4009),   /* 16393 */
        /* 164 */   NdrFcShort(0x24a),   /* Offset= 586 (750) */
        /* 166 */   NdrFcLong(0x6000),   /* 24576 */
        /* 170 */   NdrFcShort(0x248),   /* Offset= 584 (754) */
        /* 172 */   NdrFcLong(0x400c),   /* 16396 */
        /* 176 */   NdrFcShort(0x246),   /* Offset= 582 (758) */
        /* 178 */   NdrFcLong(0x10),     /* 16 */
        /* 182 */   NdrFcShort(0xffff8002),      /* Offset= -32766 (-32584) */
        /* 184 */   NdrFcLong(0x12),     /* 18 */
        /* 188 */   NdrFcShort(0xffff8006),      /* Offset= -32762 (-32574) */
        /* 190 */   NdrFcLong(0x13),     /* 19 */
        /* 194 */   NdrFcShort(0xffff8008),      /* Offset= -32760 (-32566) */
        /* 196 */   NdrFcLong(0x16),     /* 22 */
        /* 200 */   NdrFcShort(0xffff8008),      /* Offset= -32760 (-32560) */
        /* 202 */   NdrFcLong(0x17),     /* 23 */
        /* 206 */   NdrFcShort(0xffff8008),      /* Offset= -32760 (-32554) */
        /* 208 */   NdrFcLong(0xe),      /* 14 */
        /* 212 */   NdrFcShort(0x22a),   /* Offset= 554 (766) */
        /* 214 */   NdrFcLong(0x400e),   /* 16398 */
        /* 218 */   NdrFcShort(0x230),   /* Offset= 560 (778) */
        /* 220 */   NdrFcLong(0x4010),   /* 16400 */
        /* 224 */   NdrFcShort(0x1ee),   /* Offset= 494 (718) */
        /* 226 */   NdrFcLong(0x4012),   /* 16402 */
        /* 230 */   NdrFcShort(0x1ec),   /* Offset= 492 (722) */
        /* 232 */   NdrFcLong(0x4013),   /* 16403 */
        /* 236 */   NdrFcShort(0x1ea),   /* Offset= 490 (726) */
        /* 238 */   NdrFcLong(0x4016),   /* 16406 */
        /* 242 */   NdrFcShort(0x1e4),   /* Offset= 484 (726) */
        /* 244 */   NdrFcLong(0x4017),   /* 16407 */
        /* 248 */   NdrFcShort(0x1de),   /* Offset= 478 (726) */
        /* 250 */   NdrFcLong(0x0),      /* 0 */
        /* 254 */   NdrFcShort(0x0),     /* Offset= 0 (254) */
        /* 256 */   NdrFcLong(0x1),      /* 1 */
        /* 260 */   NdrFcShort(0x0),     /* Offset= 0 (260) */
        /* 262 */   NdrFcShort(0xffffffff),      /* Offset= -1 (261) */
        /* 264 */
        0x15,       /* FC_STRUCT */
        0x7,        /* 7 */
        /* 266 */   NdrFcShort(0x8),     /* 8 */
        /* 268 */   0xb,        /* FC_HYPER */
        0x5b,       /* FC_END */
        /* 270 */
        0x12, 0x0,  /* FC_UP */
        /* 272 */   NdrFcShort(0xc),     /* Offset= 12 (284) */
        /* 274 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 276 */   NdrFcShort(0x2),     /* 2 */
        /* 278 */   0x9,        /* 9 */
        0x0,        /*  */
        /* 280 */   NdrFcShort(0xfffffffc),      /* -4 */
        /* 282 */   0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 284 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 286 */   NdrFcShort(0x8),     /* 8 */
        /* 288 */   NdrFcShort(0xfffffff2),      /* Offset= -14 (274) */
        /* 290 */   0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 292 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 294 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 296 */   NdrFcLong(0x0),      /* 0 */
        /* 300 */   NdrFcShort(0x0),     /* 0 */
        /* 302 */   NdrFcShort(0x0),     /* 0 */
        /* 304 */   0xc0,       /* 192 */
        0x0,        /* 0 */
        /* 306 */   0x0,        /* 0 */
        0x0,        /* 0 */
        /* 308 */   0x0,        /* 0 */
        0x0,        /* 0 */
        /* 310 */   0x0,        /* 0 */
        0x46,       /* 70 */
        /* 312 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 314 */   NdrFcLong(0x20400),      /* 132096 */
        /* 318 */   NdrFcShort(0x0),     /* 0 */
        /* 320 */   NdrFcShort(0x0),     /* 0 */
        /* 322 */   0xc0,       /* 192 */
        0x0,        /* 0 */
        /* 324 */   0x0,        /* 0 */
        0x0,        /* 0 */
        /* 326 */   0x0,        /* 0 */
        0x0,        /* 0 */
        /* 328 */   0x0,        /* 0 */
        0x46,       /* 70 */
        /* 330 */
        0x12, 0x0,  /* FC_UP */
        /* 332 */   NdrFcShort(0x170),   /* Offset= 368 (700) */
        /* 334 */
        0x2a,       /* FC_ENCAPSULATED_UNION */
        0x48,       /* 72 */
        /* 336 */   NdrFcShort(0x8),     /* 8 */
        /* 338 */   NdrFcShort(0x8),     /* 8 */
        /* 340 */   NdrFcLong(0x8),      /* 8 */
        /* 344 */   NdrFcShort(0x4c),    /* Offset= 76 (420) */
        /* 346 */   NdrFcLong(0xd),      /* 13 */
        /* 350 */   NdrFcShort(0x6c),    /* Offset= 108 (458) */
        /* 352 */   NdrFcLong(0x9),      /* 9 */
        /* 356 */   NdrFcShort(0x88),    /* Offset= 136 (492) */
        /* 358 */   NdrFcLong(0xc),      /* 12 */
        /* 362 */   NdrFcShort(0xb0),    /* Offset= 176 (538) */
        /* 364 */   NdrFcLong(0x10),     /* 16 */
        /* 368 */   NdrFcShort(0xc8),    /* Offset= 200 (568) */
        /* 370 */   NdrFcLong(0x2),      /* 2 */
        /* 374 */   NdrFcShort(0xe0),    /* Offset= 224 (598) */
        /* 376 */   NdrFcLong(0x3),      /* 3 */
        /* 380 */   NdrFcShort(0xf8),    /* Offset= 248 (628) */
        /* 382 */   NdrFcLong(0x14),     /* 20 */
        /* 386 */   NdrFcShort(0x110),   /* Offset= 272 (658) */
        /* 388 */   NdrFcShort(0x0),     /* Offset= 0 (388) */
        /* 390 */
        0x1b,       /* FC_CARRAY */
        0x3,        /* 3 */
        /* 392 */   NdrFcShort(0x4),     /* 4 */
        /* 394 */   0x18,       /* 24 */
        0x0,        /*  */
        /* 396 */   NdrFcShort(0x0),     /* 0 */
        /* 398 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 400 */
        0x48,       /* FC_VARIABLE_REPEAT */
        0x49,       /* FC_FIXED_OFFSET */
        /* 402 */   NdrFcShort(0x4),     /* 4 */
        /* 404 */   NdrFcShort(0x0),     /* 0 */
        /* 406 */   NdrFcShort(0x1),     /* 1 */
        /* 408 */   NdrFcShort(0x0),     /* 0 */
        /* 410 */   NdrFcShort(0x0),     /* 0 */
        /* 412 */   0x12, 0x0,  /* FC_UP */
        /* 414 */   NdrFcShort(0xffffff7e),      /* Offset= -130 (284) */
        /* 416 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 418 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 420 */
        0x16,       /* FC_PSTRUCT */
        0x3,        /* 3 */
        /* 422 */   NdrFcShort(0x8),     /* 8 */
        /* 424 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 426 */
        0x46,       /* FC_NO_REPEAT */
        0x5c,       /* FC_PAD */
        /* 428 */   NdrFcShort(0x4),     /* 4 */
        /* 430 */   NdrFcShort(0x4),     /* 4 */
        /* 432 */   0x11, 0x0,  /* FC_RP */
        /* 434 */   NdrFcShort(0xffffffd4),      /* Offset= -44 (390) */
        /* 436 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 438 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 440 */
        0x21,       /* FC_BOGUS_ARRAY */
        0x3,        /* 3 */
        /* 442 */   NdrFcShort(0x0),     /* 0 */
        /* 444 */   0x18,       /* 24 */
        0x0,        /*  */
        /* 446 */   NdrFcShort(0x0),     /* 0 */
        /* 448 */   NdrFcLong(0xffffffff),   /* -1 */
        /* 452 */   0x4c,       /* FC_EMBEDDED_COMPLEX */
        0x0,        /* 0 */
        /* 454 */   NdrFcShort(0xffffff60),      /* Offset= -160 (294) */
        /* 456 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 458 */
        0x1a,       /* FC_BOGUS_STRUCT */
        0x3,        /* 3 */
        /* 460 */   NdrFcShort(0x8),     /* 8 */
        /* 462 */   NdrFcShort(0x0),     /* 0 */
        /* 464 */   NdrFcShort(0x6),     /* Offset= 6 (470) */
        /* 466 */   0x8,        /* FC_LONG */
        0x36,       /* FC_POINTER */
        /* 468 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 470 */
        0x11, 0x0,  /* FC_RP */
        /* 472 */   NdrFcShort(0xffffffe0),      /* Offset= -32 (440) */
        /* 474 */
        0x21,       /* FC_BOGUS_ARRAY */
        0x3,        /* 3 */
        /* 476 */   NdrFcShort(0x0),     /* 0 */
        /* 478 */   0x18,       /* 24 */
        0x0,        /*  */
        /* 480 */   NdrFcShort(0x0),     /* 0 */
        /* 482 */   NdrFcLong(0xffffffff),   /* -1 */
        /* 486 */   0x4c,       /* FC_EMBEDDED_COMPLEX */
        0x0,        /* 0 */
        /* 488 */   NdrFcShort(0xffffff50),      /* Offset= -176 (312) */
        /* 490 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 492 */
        0x1a,       /* FC_BOGUS_STRUCT */
        0x3,        /* 3 */
        /* 494 */   NdrFcShort(0x8),     /* 8 */
        /* 496 */   NdrFcShort(0x0),     /* 0 */
        /* 498 */   NdrFcShort(0x6),     /* Offset= 6 (504) */
        /* 500 */   0x8,        /* FC_LONG */
        0x36,       /* FC_POINTER */
        /* 502 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 504 */
        0x11, 0x0,  /* FC_RP */
        /* 506 */   NdrFcShort(0xffffffe0),      /* Offset= -32 (474) */
        /* 508 */
        0x1b,       /* FC_CARRAY */
        0x3,        /* 3 */
        /* 510 */   NdrFcShort(0x4),     /* 4 */
        /* 512 */   0x18,       /* 24 */
        0x0,        /*  */
        /* 514 */   NdrFcShort(0x0),     /* 0 */
        /* 516 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 518 */
        0x48,       /* FC_VARIABLE_REPEAT */
        0x49,       /* FC_FIXED_OFFSET */
        /* 520 */   NdrFcShort(0x4),     /* 4 */
        /* 522 */   NdrFcShort(0x0),     /* 0 */
        /* 524 */   NdrFcShort(0x1),     /* 1 */
        /* 526 */   NdrFcShort(0x0),     /* 0 */
        /* 528 */   NdrFcShort(0x0),     /* 0 */
        /* 530 */   0x12, 0x0,  /* FC_UP */
        /* 532 */   NdrFcShort(0xfa),    /* Offset= 250 (782) */
        /* 534 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 536 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 538 */
        0x16,       /* FC_PSTRUCT */
        0x3,        /* 3 */
        /* 540 */   NdrFcShort(0x8),     /* 8 */
        /* 542 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 544 */
        0x46,       /* FC_NO_REPEAT */
        0x5c,       /* FC_PAD */
        /* 546 */   NdrFcShort(0x4),     /* 4 */
        /* 548 */   NdrFcShort(0x4),     /* 4 */
        /* 550 */   0x11, 0x0,  /* FC_RP */
        /* 552 */   NdrFcShort(0xffffffd4),      /* Offset= -44 (508) */
        /* 554 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 556 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 558 */
        0x1b,       /* FC_CARRAY */
        0x0,        /* 0 */
        /* 560 */   NdrFcShort(0x1),     /* 1 */
        /* 562 */   0x19,       /* 25 */
        0x0,        /*  */
        /* 564 */   NdrFcShort(0x0),     /* 0 */
        /* 566 */   0x1,        /* FC_BYTE */
        0x5b,       /* FC_END */
        /* 568 */
        0x16,       /* FC_PSTRUCT */
        0x3,        /* 3 */
        /* 570 */   NdrFcShort(0x8),     /* 8 */
        /* 572 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 574 */
        0x46,       /* FC_NO_REPEAT */
        0x5c,       /* FC_PAD */
        /* 576 */   NdrFcShort(0x4),     /* 4 */
        /* 578 */   NdrFcShort(0x4),     /* 4 */
        /* 580 */   0x12, 0x0,  /* FC_UP */
        /* 582 */   NdrFcShort(0xffffffe8),      /* Offset= -24 (558) */
        /* 584 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 586 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 588 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 590 */   NdrFcShort(0x2),     /* 2 */
        /* 592 */   0x19,       /* 25 */
        0x0,        /*  */
        /* 594 */   NdrFcShort(0x0),     /* 0 */
        /* 596 */   0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 598 */
        0x16,       /* FC_PSTRUCT */
        0x3,        /* 3 */
        /* 600 */   NdrFcShort(0x8),     /* 8 */
        /* 602 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 604 */
        0x46,       /* FC_NO_REPEAT */
        0x5c,       /* FC_PAD */
        /* 606 */   NdrFcShort(0x4),     /* 4 */
        /* 608 */   NdrFcShort(0x4),     /* 4 */
        /* 610 */   0x12, 0x0,  /* FC_UP */
        /* 612 */   NdrFcShort(0xffffffe8),      /* Offset= -24 (588) */
        /* 614 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 616 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 618 */
        0x1b,       /* FC_CARRAY */
        0x3,        /* 3 */
        /* 620 */   NdrFcShort(0x4),     /* 4 */
        /* 622 */   0x19,       /* 25 */
        0x0,        /*  */
        /* 624 */   NdrFcShort(0x0),     /* 0 */
        /* 626 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 628 */
        0x16,       /* FC_PSTRUCT */
        0x3,        /* 3 */
        /* 630 */   NdrFcShort(0x8),     /* 8 */
        /* 632 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 634 */
        0x46,       /* FC_NO_REPEAT */
        0x5c,       /* FC_PAD */
        /* 636 */   NdrFcShort(0x4),     /* 4 */
        /* 638 */   NdrFcShort(0x4),     /* 4 */
        /* 640 */   0x12, 0x0,  /* FC_UP */
        /* 642 */   NdrFcShort(0xffffffe8),      /* Offset= -24 (618) */
        /* 644 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 646 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 648 */
        0x1b,       /* FC_CARRAY */
        0x7,        /* 7 */
        /* 650 */   NdrFcShort(0x8),     /* 8 */
        /* 652 */   0x19,       /* 25 */
        0x0,        /*  */
        /* 654 */   NdrFcShort(0x0),     /* 0 */
        /* 656 */   0xb,        /* FC_HYPER */
        0x5b,       /* FC_END */
        /* 658 */
        0x16,       /* FC_PSTRUCT */
        0x3,        /* 3 */
        /* 660 */   NdrFcShort(0x8),     /* 8 */
        /* 662 */
        0x4b,       /* FC_PP */
        0x5c,       /* FC_PAD */
        /* 664 */
        0x46,       /* FC_NO_REPEAT */
        0x5c,       /* FC_PAD */
        /* 666 */   NdrFcShort(0x4),     /* 4 */
        /* 668 */   NdrFcShort(0x4),     /* 4 */
        /* 670 */   0x12, 0x0,  /* FC_UP */
        /* 672 */   NdrFcShort(0xffffffe8),      /* Offset= -24 (648) */
        /* 674 */
        0x5b,       /* FC_END */

        0x8,        /* FC_LONG */
        /* 676 */   0x8,        /* FC_LONG */
        0x5b,       /* FC_END */
        /* 678 */
        0x15,       /* FC_STRUCT */
        0x3,        /* 3 */
        /* 680 */   NdrFcShort(0x8),     /* 8 */
        /* 682 */   0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 684 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 686 */
        0x1b,       /* FC_CARRAY */
        0x3,        /* 3 */
        /* 688 */   NdrFcShort(0x8),     /* 8 */
        /* 690 */   0x6,        /* 6 */
        0x0,        /*  */
        /* 692 */   NdrFcShort(0xffffffe8),      /* -24 */
        /* 694 */   0x4c,       /* FC_EMBEDDED_COMPLEX */
        0x0,        /* 0 */
        /* 696 */   NdrFcShort(0xffffffee),      /* Offset= -18 (678) */
        /* 698 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 700 */
        0x1a,       /* FC_BOGUS_STRUCT */
        0x3,        /* 3 */
        /* 702 */   NdrFcShort(0x18),    /* 24 */
        /* 704 */   NdrFcShort(0xffffffee),      /* Offset= -18 (686) */
        /* 706 */   NdrFcShort(0x0),     /* Offset= 0 (706) */
        /* 708 */   0x6,        /* FC_SHORT */
        0x6,        /* FC_SHORT */
        /* 710 */   0x38,       /* FC_ALIGNM4 */
        0x8,        /* FC_LONG */
        /* 712 */   0x8,        /* FC_LONG */
        0x4c,       /* FC_EMBEDDED_COMPLEX */
        /* 714 */   0x0,        /* 0 */
        NdrFcShort(0xfffffe83),      /* Offset= -381 (334) */
        0x5b,       /* FC_END */
        /* 718 */
        0x12, 0x8,  /* FC_UP [simple_pointer] */
        /* 720 */   0x2,        /* FC_CHAR */
        0x5c,       /* FC_PAD */
        /* 722 */
        0x12, 0x8,  /* FC_UP [simple_pointer] */
        /* 724 */   0x6,        /* FC_SHORT */
        0x5c,       /* FC_PAD */
        /* 726 */
        0x12, 0x8,  /* FC_UP [simple_pointer] */
        /* 728 */   0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */
        /* 730 */
        0x12, 0x8,  /* FC_UP [simple_pointer] */
        /* 732 */   0xa,        /* FC_FLOAT */
        0x5c,       /* FC_PAD */
        /* 734 */
        0x12, 0x8,  /* FC_UP [simple_pointer] */
        /* 736 */   0xc,        /* FC_DOUBLE */
        0x5c,       /* FC_PAD */
        /* 738 */
        0x12, 0x0,  /* FC_UP */
        /* 740 */   NdrFcShort(0xfffffe24),      /* Offset= -476 (264) */
        /* 742 */
        0x12, 0x10, /* FC_UP */
        /* 744 */   NdrFcShort(0xfffffe26),      /* Offset= -474 (270) */
        /* 746 */
        0x12, 0x10, /* FC_UP */
        /* 748 */   NdrFcShort(0xfffffe3a),      /* Offset= -454 (294) */
        /* 750 */
        0x12, 0x10, /* FC_UP */
        /* 752 */   NdrFcShort(0xfffffe48),      /* Offset= -440 (312) */
        /* 754 */
        0x12, 0x10, /* FC_UP */
        /* 756 */   NdrFcShort(0xfffffe56),      /* Offset= -426 (330) */
        /* 758 */
        0x12, 0x10, /* FC_UP */
        /* 760 */   NdrFcShort(0x2),     /* Offset= 2 (762) */
        /* 762 */
        0x12, 0x0,  /* FC_UP */
        /* 764 */   NdrFcShort(0xfffffd04),      /* Offset= -764 (0) */
        /* 766 */
        0x15,       /* FC_STRUCT */
        0x7,        /* 7 */
        /* 768 */   NdrFcShort(0x10),    /* 16 */
        /* 770 */   0x6,        /* FC_SHORT */
        0x2,        /* FC_CHAR */
        /* 772 */   0x2,        /* FC_CHAR */
        0x38,       /* FC_ALIGNM4 */
        /* 774 */   0x8,        /* FC_LONG */
        0x39,       /* FC_ALIGNM8 */
        /* 776 */   0xb,        /* FC_HYPER */
        0x5b,       /* FC_END */
        /* 778 */
        0x12, 0x0,  /* FC_UP */
        /* 780 */   NdrFcShort(0xfffffff2),      /* Offset= -14 (766) */
        /* 782 */
        0x1a,       /* FC_BOGUS_STRUCT */
        0x7,        /* 7 */
        /* 784 */   NdrFcShort(0x20),    /* 32 */
        /* 786 */   NdrFcShort(0x0),     /* 0 */
        /* 788 */   NdrFcShort(0x0),     /* Offset= 0 (788) */
        /* 790 */   0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 792 */   0x6,        /* FC_SHORT */
        0x6,        /* FC_SHORT */
        /* 794 */   0x6,        /* FC_SHORT */
        0x6,        /* FC_SHORT */
        /* 796 */   0x4c,       /* FC_EMBEDDED_COMPLEX */
        0x0,        /* 0 */
        /* 798 */   NdrFcShort(0xfffffce6),      /* Offset= -794 (4) */
        /* 800 */   0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 802 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 804 */   NdrFcShort(0x0),     /* 0 */
        /* 806 */   NdrFcShort(0x10),    /* 16 */
        /* 808 */   NdrFcShort(0x0),     /* 0 */
        /* 810 */   NdrFcShort(0xfffffcd6),      /* Offset= -810 (0) */
        /* 812 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 814 */   NdrFcShort(0x0),     /* 0 */
        /* 816 */   NdrFcShort(0x10),    /* 16 */
        /* 818 */   NdrFcShort(0x0),     /* 0 */
        /* 820 */   NdrFcShort(0xfffffccc),      /* Offset= -820 (0) */
        /* 822 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 824 */   NdrFcShort(0x6),     /* Offset= 6 (830) */
        /* 826 */
        0x13, 0x0,  /* FC_OP */
        /* 828 */   NdrFcShort(0xffffffd2),      /* Offset= -46 (782) */
        /* 830 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 832 */   NdrFcShort(0x0),     /* 0 */
        /* 834 */   NdrFcShort(0x10),    /* 16 */
        /* 836 */   NdrFcShort(0x0),     /* 0 */
        /* 838 */   NdrFcShort(0xfffffff4),      /* Offset= -12 (826) */
        /* 840 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 842 */   NdrFcShort(0x0),     /* 0 */
        /* 844 */   NdrFcShort(0x10),    /* 16 */
        /* 846 */   NdrFcShort(0x0),     /* 0 */
        /* 848 */   NdrFcShort(0xfffffcb0),      /* Offset= -848 (0) */
        /* 850 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 852 */   NdrFcShort(0x0),     /* 0 */
        /* 854 */   NdrFcShort(0x10),    /* 16 */
        /* 856 */   NdrFcShort(0x0),     /* 0 */
        /* 858 */   NdrFcShort(0xfffffca6),      /* Offset= -858 (0) */
        /* 860 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 862 */   NdrFcShort(0x2),     /* Offset= 2 (864) */
        /* 864 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 866 */   NdrFcShort(0x0),     /* 0 */
        /* 868 */   NdrFcShort(0x10),    /* 16 */
        /* 870 */   NdrFcShort(0x0),     /* 0 */
        /* 872 */   NdrFcShort(0xffffffd2),      /* Offset= -46 (826) */
        /* 874 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 876 */   NdrFcShort(0x0),     /* 0 */
        /* 878 */   NdrFcShort(0x10),    /* 16 */
        /* 880 */   NdrFcShort(0x0),     /* 0 */
        /* 882 */   NdrFcShort(0xfffffc8e),      /* Offset= -882 (0) */
        /* 884 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 886 */   NdrFcShort(0x0),     /* 0 */
        /* 888 */   NdrFcShort(0x10),    /* 16 */
        /* 890 */   NdrFcShort(0x0),     /* 0 */
        /* 892 */   NdrFcShort(0xfffffc84),      /* Offset= -892 (0) */
        /* 894 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 896 */   NdrFcShort(0x2),     /* Offset= 2 (898) */
        /* 898 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 900 */   NdrFcShort(0x0),     /* 0 */
        /* 902 */   NdrFcShort(0x10),    /* 16 */
        /* 904 */   NdrFcShort(0x0),     /* 0 */
        /* 906 */   NdrFcShort(0xffffffb0),      /* Offset= -80 (826) */
        /* 908 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 910 */   NdrFcShort(0x0),     /* 0 */
        /* 912 */   NdrFcShort(0x10),    /* 16 */
        /* 914 */   NdrFcShort(0x0),     /* 0 */
        /* 916 */   NdrFcShort(0xfffffc6c),      /* Offset= -916 (0) */
        /* 918 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 920 */   NdrFcShort(0x0),     /* 0 */
        /* 922 */   NdrFcShort(0x10),    /* 16 */
        /* 924 */   NdrFcShort(0x0),     /* 0 */
        /* 926 */   NdrFcShort(0xfffffc62),      /* Offset= -926 (0) */
        /* 928 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 930 */   NdrFcShort(0x2),     /* Offset= 2 (932) */
        /* 932 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 934 */   NdrFcShort(0x0),     /* 0 */
        /* 936 */   NdrFcShort(0x10),    /* 16 */
        /* 938 */   NdrFcShort(0x0),     /* 0 */
        /* 940 */   NdrFcShort(0xffffff8e),      /* Offset= -114 (826) */
        /* 942 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 944 */   NdrFcShort(0x0),     /* 0 */
        /* 946 */   NdrFcShort(0x10),    /* 16 */
        /* 948 */   NdrFcShort(0x0),     /* 0 */
        /* 950 */   NdrFcShort(0xfffffc4a),      /* Offset= -950 (0) */
        /* 952 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 954 */   NdrFcShort(0x0),     /* 0 */
        /* 956 */   NdrFcShort(0x10),    /* 16 */
        /* 958 */   NdrFcShort(0x0),     /* 0 */
        /* 960 */   NdrFcShort(0xfffffc40),      /* Offset= -960 (0) */
        /* 962 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 964 */   NdrFcShort(0x2),     /* Offset= 2 (966) */
        /* 966 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 968 */   NdrFcShort(0x0),     /* 0 */
        /* 970 */   NdrFcShort(0x10),    /* 16 */
        /* 972 */   NdrFcShort(0x0),     /* 0 */
        /* 974 */   NdrFcShort(0xffffff6c),      /* Offset= -148 (826) */
        /* 976 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 978 */   NdrFcShort(0x0),     /* 0 */
        /* 980 */   NdrFcShort(0x10),    /* 16 */
        /* 982 */   NdrFcShort(0x0),     /* 0 */
        /* 984 */   NdrFcShort(0xfffffc28),      /* Offset= -984 (0) */
        /* 986 */   0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 988 */   NdrFcShort(0x0),     /* 0 */
        /* 990 */   NdrFcShort(0x10),    /* 16 */
        /* 992 */   NdrFcShort(0x0),     /* 0 */
        /* 994 */   NdrFcShort(0xfffffc1e),      /* Offset= -994 (0) */
        /* 996 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 998 */   NdrFcShort(0x2),     /* Offset= 2 (1000) */
        /* 1000 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1002 */  NdrFcShort(0x0),     /* 0 */
        /* 1004 */  NdrFcShort(0x10),    /* 16 */
        /* 1006 */  NdrFcShort(0x0),     /* 0 */
        /* 1008 */  NdrFcShort(0xffffff4a),      /* Offset= -182 (826) */
        /* 1010 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1012 */  NdrFcShort(0x0),     /* 0 */
        /* 1014 */  NdrFcShort(0x10),    /* 16 */
        /* 1016 */  NdrFcShort(0x0),     /* 0 */
        /* 1018 */  NdrFcShort(0xfffffc06),      /* Offset= -1018 (0) */
        /* 1020 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1022 */  NdrFcShort(0x0),     /* 0 */
        /* 1024 */  NdrFcShort(0x10),    /* 16 */
        /* 1026 */  NdrFcShort(0x0),     /* 0 */
        /* 1028 */  NdrFcShort(0xfffffbfc),      /* Offset= -1028 (0) */
        /* 1030 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 1032 */  NdrFcShort(0x2),     /* Offset= 2 (1034) */
        /* 1034 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1036 */  NdrFcShort(0x0),     /* 0 */
        /* 1038 */  NdrFcShort(0x10),    /* 16 */
        /* 1040 */  NdrFcShort(0x0),     /* 0 */
        /* 1042 */  NdrFcShort(0xffffff28),      /* Offset= -216 (826) */
        /* 1044 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1046 */  NdrFcShort(0x0),     /* 0 */
        /* 1048 */  NdrFcShort(0x10),    /* 16 */
        /* 1050 */  NdrFcShort(0x0),     /* 0 */
        /* 1052 */  NdrFcShort(0xfffffbe4),      /* Offset= -1052 (0) */
        /* 1054 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1056 */  NdrFcShort(0x0),     /* 0 */
        /* 1058 */  NdrFcShort(0x10),    /* 16 */
        /* 1060 */  NdrFcShort(0x0),     /* 0 */
        /* 1062 */  NdrFcShort(0xfffffbda),      /* Offset= -1062 (0) */
        /* 1064 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 1066 */  NdrFcShort(0x2),     /* Offset= 2 (1068) */
        /* 1068 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1070 */  NdrFcShort(0x0),     /* 0 */
        /* 1072 */  NdrFcShort(0x10),    /* 16 */
        /* 1074 */  NdrFcShort(0x0),     /* 0 */
        /* 1076 */  NdrFcShort(0xffffff06),      /* Offset= -250 (826) */
        /* 1078 */  0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 1080 */  NdrFcShort(0x0),     /* 0 */
        /* 1082 */  NdrFcShort(0x10),    /* 16 */
        /* 1084 */  NdrFcShort(0x0),     /* 0 */
        /* 1086 */  NdrFcShort(0xfffffbc2),      /* Offset= -1086 (0) */
        /* 1088 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 1090 */  0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter6_Server_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IMath2ProxyVtbl,
    (CInterfaceProxyVtbl *) &_IAdvancedMathProxyVtbl,
    (CInterfaceProxyVtbl *) &_IMathProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter6_Server_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IMath2StubVtbl,
    (CInterfaceStubVtbl *) &_IAdvancedMathStubVtbl,
    (CInterfaceStubVtbl *) &_IMathStubVtbl,
    0
};

PCInterfaceName const _Chapter6_Server_InterfaceNamesList[] = {
    "IMath2",
    "IAdvancedMath",
    "IMath",
    0
};

const IID *  _Chapter6_Server_BaseIIDList[] = {
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _Chapter6_Server_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _Chapter6_Server, pIID, n)

int __stdcall _Chapter6_Server_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_Chapter6_Server, 3, 2)
    IID_BS_LOOKUP_NEXT_TEST(_Chapter6_Server, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_Chapter6_Server, 3, *pIndex)
}

const ExtendedProxyFileInfo Chapter6_Server_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter6_Server_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter6_Server_StubVtblList,
    (const PCInterfaceName *) & _Chapter6_Server_InterfaceNamesList,
    (const IID **) & _Chapter6_Server_BaseIIDList,
    & _Chapter6_Server_IID_Lookup,
    3,
    2
};
